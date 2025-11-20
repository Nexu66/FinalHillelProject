#include "coreImpl.hpp"

namespace Core {
namespace impl {
const qsizetype CollatzProcessorImpl::s_CoresCount =
    QThread::idealThreadCount();
std::vector<std::jthread> CollatzProcessorImpl::s_ThreadPool{
    static_cast<size_t>(CollatzProcessorImpl::s_CoresCount)};
QHash<qsizetype, qsizetype> CollatzProcessorImpl::s_PreCalculatedValues;
std::atomic<qsizetype> CollatzProcessorImpl::Elements = 0;

void CollatzProcessorImpl::RequestStop() {
  s_ThreadPool.begin()->request_stop();
}

bool CollatzProcessorImpl::WillOverflow(qsizetype current_element) {
  if (current_element > this->s_MaxSize / 3 + 1) is_Overflow = true;
  return is_Overflow;
}

qsizetype CollatzProcessorImpl::StartProcessing(
    std::stop_token stop, const qsizetype CurrentThreadLimit,
    const qsizetype CurrentUpperLimit) {
  this->Elements = CurrentUpperLimit;
  std::cout << "CURRENT CORES IN coreImpl: " << CurrentThreadLimit << "\n";
  Timer.StartTimer();
  for (int i = 0; i < CurrentThreadLimit; ++i) {
    s_ThreadPool[i] =
        std::jthread{&CollatzProcessorImpl::Run, this, stop, CurrentUpperLimit};
  }

  for (int i = 0; i < CurrentThreadLimit; ++i) {
    s_ThreadPool[i].join();
  }
  Elements = 0;

  if (stop.stop_requested() && is_Overflow) return Signals::OVERFLOW;
  if (stop.stop_requested()) return Signals::STOP;

  qsizetype res = FindFinalResult();
  std::cout << "Out time is: " << Timer.StopTimer() << "ms\n";
  std::cout << "Count: " << res << "\n";

  return res;
}

qsizetype CollatzProcessorImpl::CalculateCollatz(qsizetype current_element) {
  qsizetype original_element = current_element;
  qsizetype step_counter = 0;
  std::pair<qsizetype, qsizetype> local_result{current_element, step_counter};
  while (current_element != 1) {
    if (current_element % 2) {
      current_element = current_element * 3 + 1;
      if (WillOverflow(current_element)) return OVERFLOW;
    } else {
      current_element /= 2;
    }
    ++step_counter;
  }
  return step_counter;
}

void CollatzProcessorImpl::SaveFinalThreadResult(
    qsizetype final_thread_result) {
  std::lock_guard<std::mutex> write_access_lock{ThreadResultsLock};
  ThreadResults.push_back(final_thread_result);
  // for (auto p : ThreadResults) {
  //   std::cout << "Val: " << p.first << " Count: " << p.second << "\n";
  // }
}

qsizetype CollatzProcessorImpl::FindFinalResult() {
  auto result = *std::ranges::max_element(
      ThreadResults, [](const qsizetype first_el, const qsizetype second_el) {
        return first_el < second_el;
      });
  ThreadResults.clear();
  return result;
}

void CollatzProcessorImpl::Run(std::stop_token stop,
                               const qsizetype CurrentUpperLimit) {
  qsizetype current_element = Elements--;
  qsizetype local_thread_result = 0;

  qsizetype current_result;
  while (current_element >= 1) {
    current_result = CalculateCollatz(current_element);
    if (current_result == Signals::OVERFLOW) {
      RequestStop();
    }
    if (stop.stop_requested()) return;
    if (current_result > local_thread_result)
      local_thread_result = current_result;

    current_element = Elements--;
  }
  SaveFinalThreadResult(local_thread_result);
}
}  // namespace impl
}  // namespace Core
