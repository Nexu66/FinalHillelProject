#include "coreImpl.hpp"

namespace Core {
namespace impl {

const qsizetype CollatzProcessorImpl::cs_CoresCount =
    QThread::idealThreadCount();
std::vector<std::jthread> CollatzProcessorImpl::s_ThreadPool{
    static_cast<size_t>(CollatzProcessorImpl::cs_CoresCount)};

timer::Timer CollatzProcessorImpl::s_Timer{};

std::atomic<qsizetype>* CollatzProcessorImpl::s_Cache =
    new std::atomic<qsizetype>[cs_UpperLimitCap] { 0 };

CollatzProcessorImpl::CollatzProcessorImpl() {
  s_ThreadResults.resize(cs_CoresCount);
}

CollatzProcessorImpl::~CollatzProcessorImpl() { delete[] s_Cache; }

std::vector<ResultWrapper> CollatzProcessorImpl::s_ThreadResults{};

void CollatzProcessorImpl::RequestStop() {
  s_ThreadPool.begin()->request_stop();
}

bool CollatzProcessorImpl::WillOverflow(qsizetype current_element) {
  if (current_element > this->cs_MaxSizeBeforeOverflow) is_Overflow = true;
  return is_Overflow;
}

std::pair<qsizetype, qsizetype> CollatzProcessorImpl::StartProcessing(
    std::stop_token stop, const qsizetype CurrentThreadLimit,
    const qsizetype CurrentUpperLimit) {
  std::cout << "CURRENT CORES IN coreImpl: " << CurrentThreadLimit << "\n";
  s_Timer.StartTimer();
  for (int i = 0; i < CurrentThreadLimit; ++i) {
    s_ThreadPool[i] = std::jthread{&CollatzProcessorImpl::Run, this, stop,
                                   CurrentUpperLimit, i};
  }

  for (int i = 0; i < CurrentThreadLimit; ++i) {
    s_ThreadPool[i].join();
  }

  if (stop.stop_requested() && is_Overflow)
    return std::make_pair(Signals::VALUE_OVERFLOWED, Signals::VALUE_OVERFLOWED);
  if (stop.stop_requested())
    return std::make_pair(Signals::STOP, Signals::STOP);

  std::pair<qsizetype, qsizetype> res = FindFinalResult();
  std::cout << "Out time is: " << s_Timer.StopTimer() << "ms\n";
  std::cout << "Num: " << res.first << " Count: " << res.second << "\n";

  return res;
}
void CollatzProcessorImpl::Run(std::stop_token stop,
                               const qsizetype CurrentUpperLimit,
                               const qsizetype IndexInResultsVector) {
  const qsizetype StepInCachedVector = IndexInResultsVector + 1;
  qsizetype current_element = CurrentUpperLimit + 1 - StepInCachedVector;
  qsizetype& result_element =
      s_ThreadResults[IndexInResultsVector].result.first;
  qsizetype& result_step_counter =
      s_ThreadResults[IndexInResultsVector].result.second;

  while (current_element > 1) {
    if (s_Cache[current_element - 1].load() == 0) {
      CalculateCollatz(current_element, 0);
    }
    if (result_step_counter < s_Cache[current_element - 1].load()) {
      result_step_counter = s_Cache[current_element - 1].load();
      result_element = current_element;
      result_step_counter = s_Cache[current_element - 1].load();
    }

    if (stop.stop_requested()) return;
    current_element -= StepInCachedVector;
  }
  return;
}

qsizetype CollatzProcessorImpl::CalculateCollatz(qsizetype current_element,
                                                 qsizetype step_counter) {
  if (current_element == 1) return ++step_counter;
  if (current_element < cs_UpperLimitCap + 1 &&
      s_Cache[current_element - 1].load() != 0) {
    step_counter += s_Cache[current_element - 1].load();
    return ++step_counter;
  } else {
    ++step_counter;
  }
  if (WillOverflow(current_element)) this->RequestStop();
  qsizetype steps_it_took = CalculateCollatz(
      current_element % 2 ? current_element * 3 + 1 : current_element / 2,
      step_counter);
  if (current_element < cs_UpperLimitCap + 1 &&
      s_Cache[current_element - 1].load() == 0)
    s_Cache[current_element - 1].store(
        std::atomic<qsizetype>{steps_it_took - step_counter});
  return steps_it_took;
}

std::pair<qsizetype, qsizetype> CollatzProcessorImpl::FindFinalResult() {
  qsizetype largest_count = 0;
  qsizetype number = 1;
  for (int i = 0; i < s_ThreadResults.size(); ++i) {
    if (s_ThreadResults[i].result.second > largest_count) {
      largest_count = s_ThreadResults[i].result.second;
      number = s_ThreadResults[i].result.first;
    }
  }
  s_ThreadResults.assign(s_ThreadResults.size(), ResultWrapper{});
  return std::make_pair(number, largest_count);
}

}  // namespace impl
}  // namespace Core
