#include "coreImpl.hpp"

namespace Core {
namespace impl {

const qsizetype CollatzProcessorImpl::cs_CoresCount =
    QThread::idealThreadCount();
std::vector<std::jthread> CollatzProcessorImpl::s_ThreadPool{
    static_cast<size_t>(CollatzProcessorImpl::cs_CoresCount)};
std::atomic<qsizetype> CollatzProcessorImpl::s_Elements = 1;


timer::Timer CollatzProcessorImpl::s_Timer{};

QList<std::atomic<qsizetype>*> CollatzProcessorImpl::s_Cache{};

CollatzProcessorImpl::CollatzProcessorImpl() {
  s_Cache.reserve(1000000);
  s_ThreadResults.resize(cs_CoresCount);
}

CollatzProcessorImpl::~CollatzProcessorImpl() { qDeleteAll(s_Cache); }

std::vector<std::pair<qsizetype, qsizetype> >
    CollatzProcessorImpl::s_ThreadResults{};

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
  std::cout << "SIZE: " << s_Cache.size() << "\n";
  const qsizetype current_size = s_Cache.size();
  for (int i = 0; i < CurrentUpperLimit - current_size; ++i) {
    s_Cache.append(new std::atomic<qsizetype>{0});
  }
  for (int i = 0; i < CurrentThreadLimit; ++i) {
    s_ThreadPool[i] = std::jthread{&CollatzProcessorImpl::Run, this, stop,
                                   CurrentUpperLimit, i};
  }

  for (int i = 0; i < CurrentThreadLimit; ++i) {
    s_ThreadPool[i].join();
  }
  s_Elements = 1;

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
  qsizetype current_element = s_Elements++;
  qsizetype result_element = 1;
  qsizetype result_step_counter = 0;

  while (current_element <= CurrentUpperLimit) {
    if (s_Cache[current_element - 1]->load() == 0 && current_element != 1) {
      CalculateCollatz(current_element);
    }
    if (result_step_counter < s_Cache[current_element - 1]->load()) {
      result_step_counter = s_Cache[current_element - 1]->load();
      result_element = current_element;
    }

    if (stop.stop_requested()) return;
    current_element = s_Elements++;
  }
  this->SaveThreadResult(result_element, result_step_counter,
                         IndexInResultsVector);
  return;
}

void CollatzProcessorImpl::CalculateCollatz(qsizetype current_element) {
  qsizetype original_element = current_element;
  qsizetype step_counter = 0;
  while (current_element > 1) {
    if (current_element % 2) {
      current_element = current_element * 3 + 1;
      if (WillOverflow(current_element)) this->RequestStop();
    } else {
      current_element /= 2;
    }
    ++step_counter;
  }
  s_Cache[original_element - 1]->store(step_counter);
}

void CollatzProcessorImpl::SaveThreadResult(
    qsizetype result_element, qsizetype result_step_count,
    const qsizetype IndexInResultsVector) {
  s_ThreadResults[IndexInResultsVector] =
      std::make_pair(result_element, result_step_count);
}

std::pair<qsizetype, qsizetype> CollatzProcessorImpl::FindFinalResult() {
  qsizetype largest_count = 0;
  qsizetype number = 1;
  for (int i = 0; i < s_ThreadResults.size(); ++i) {
    if (s_ThreadResults[i].second > largest_count) {
      largest_count = s_ThreadResults[i].second;
      number = s_ThreadResults[i].first;
    }
  }
  s_ThreadResults.assign(s_ThreadResults.size(), std::make_pair(0, 0));
  return std::make_pair(number, largest_count);
}

}  // namespace impl
}  // namespace Core
