#include "coreImpl.hpp"

namespace Core {
namespace impl {

const qsizetype CollatzProcessorImpl::cs_CoresCount =
    QThread::idealThreadCount();
std::vector<std::jthread> CollatzProcessorImpl::s_ThreadPool{
    static_cast<size_t>(CollatzProcessorImpl::cs_CoresCount)};

timer::Timer CollatzProcessorImpl::s_Timer{};

std::atomic<qsizetype>* CollatzProcessorImpl::s_pCache =
    new std::atomic<qsizetype>[cs_UpperLimitCap] { 0 };

CollatzProcessorImpl::CollatzProcessorImpl() {
  s_ThreadResults.resize(cs_CoresCount);
}

CollatzProcessorImpl::~CollatzProcessorImpl() { delete[] s_pCache; }

std::vector<std::pair<qsizetype, qsizetype>>
    CollatzProcessorImpl::s_ThreadResults{};

void CollatzProcessorImpl::RequestStop() {
  s_ThreadPool.begin()->request_stop();
}

bool CollatzProcessorImpl::WillOverflow(const qsizetype CurrentElement) {
  if (CurrentElement > this->cs_MaxSizeBeforeOverflow) IsOverflow = true;
  return IsOverflow;
}

std::pair<qsizetype, qsizetype> CollatzProcessorImpl::StartProcessing(
    const std::stop_token StopToken, const qsizetype CurrentThreadLimit,
    const qsizetype CurrentUpperLimit) {
  const qsizetype ElementsPerThread = CurrentUpperLimit / CurrentThreadLimit;
  s_Timer.StartTimer();
  int i = 0;
  for (; i < CurrentThreadLimit - 1; ++i) {
    s_ThreadPool[i] = std::jthread{
        &CollatzProcessorImpl::Run,  this, StopToken, ElementsPerThread * i,
        ElementsPerThread * (i + 1), i};
  }
  s_ThreadPool[i] = std::jthread{
      &CollatzProcessorImpl::Run, this, StopToken, ElementsPerThread * i,
      CurrentUpperLimit,          i};

  for (int i = 0; i < CurrentThreadLimit; ++i) {
    s_ThreadPool[i].join();
  }

  if (StopToken.stop_requested() && IsOverflow)
    return std::make_pair(Signals::VALUE_OVERFLOWED, Signals::VALUE_OVERFLOWED);
  if (StopToken.stop_requested())
    return std::make_pair(Signals::STOP, Signals::STOP);

  std::pair<qsizetype, qsizetype> FinalResult = FindFinalResult();
  s_Timer.StopTimer();
  return FinalResult;
}
void CollatzProcessorImpl::Run(const std::stop_token StopToken,
                               const qsizetype IntervalBegin,
                               const qsizetype IntervalEnd,
                               const qsizetype IndexInResultsVector) {
  qsizetype CurrentElement = IntervalEnd;
  qsizetype& ResultElementPointer = s_ThreadResults[IndexInResultsVector].first;
  qsizetype& ResultStepCounterPointer =
      s_ThreadResults[IndexInResultsVector].second;

  while (CurrentElement > IntervalBegin) {
    qsizetype LocalStepCounter = 0;
    if (s_pCache[CurrentElement - 1].load() == 0) {
      CalculateCollatz(CurrentElement, LocalStepCounter);
    }
    if (ResultStepCounterPointer < s_pCache[CurrentElement - 1].load()) {
      ResultStepCounterPointer = s_pCache[CurrentElement - 1].load();
      ResultElementPointer = CurrentElement;
      ResultStepCounterPointer = s_pCache[CurrentElement - 1].load();
    }

    if (StopToken.stop_requested()) return;
    --CurrentElement;
  }
  return;
}

void CollatzProcessorImpl::CalculateCollatz(qsizetype CurrentElement,
                                            qsizetype& StepCounter) {
  if (CurrentElement == 1) return;
  if (CurrentElement < cs_UpperLimitCap + 1 &&
      s_pCache[CurrentElement - 1].load() != 0) {
    StepCounter += s_pCache[CurrentElement - 1].load();
    return;
  }
  if (WillOverflow(CurrentElement)) this->RequestStop();
  CalculateCollatz(
      CurrentElement % 2 ? CurrentElement * 3 + 1 : CurrentElement / 2,
      StepCounter);
  ++StepCounter;
  if (CurrentElement < cs_UpperLimitCap + 1 &&
      s_pCache[CurrentElement - 1].load() == 0)
    s_pCache[CurrentElement - 1].store(std::atomic<qsizetype>{StepCounter});
  return;
}

std::pair<qsizetype, qsizetype> CollatzProcessorImpl::FindFinalResult() {
  qsizetype LargestCount = 0;
  qsizetype Number = 1;
  for (int i = 0; i < s_ThreadResults.size(); ++i) {
    if (s_ThreadResults[i].second > LargestCount) {
      LargestCount = s_ThreadResults[i].second;
      Number = s_ThreadResults[i].first;
    }
  }
  s_ThreadResults.assign(s_ThreadResults.size(),
                         std::pair<qsizetype, qsizetype>{0, 0});
  return std::make_pair(Number, LargestCount);
}

}  // namespace impl
}  // namespace Core
