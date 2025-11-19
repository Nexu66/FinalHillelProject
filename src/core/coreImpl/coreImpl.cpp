#include "coreImpl.hpp"

namespace Core {
namespace impl {
const qsizetype CollatzProcessorImpl::s_CoresCount =
    QThread::idealThreadCount();
std::vector<std::jthread> CollatzProcessorImpl::s_ThreadPool{
    static_cast<size_t>(CollatzProcessorImpl::s_CoresCount)};
QHash<qsizetype, qsizetype> CollatzProcessorImpl::s_CalculatedValues;
std::atomic<qsizetype> CollatzProcessorImpl::Elements = 2;

void CollatzProcessorImpl::RequestStop() {
  s_ThreadPool.begin()->request_stop();
}

bool CollatzProcessorImpl::WillOverflow(qsizetype current_element) {
  if (current_element > this->s_MaxSize / 3 + 1) is_Overflow = true;
  return is_Overflow;
}

std::pair<qsizetype, qsizetype> CollatzProcessorImpl::StartProcessing(
    std::stop_token stop, const qsizetype CurrentThreadLimit,
    const qsizetype CurrentUpperLimit) {
  std::cout << "CURRENT CORES IN coreImpl: " << CurrentThreadLimit << "\n";
  Timer.StartTimer();
  for (int i = 0; i < CurrentThreadLimit; ++i) {
    s_ThreadPool[i] =
        std::jthread{&CollatzProcessorImpl::Run, this, stop, CurrentUpperLimit};
  }

  for (int i = 0; i < CurrentThreadLimit; ++i) {
    s_ThreadPool[i].join();
  }
  Elements = 2;

  if (stop.stop_requested() && is_Overflow)
    return std::make_pair(Signals::OVERFLOW, Signals::OVERFLOW);
  if (stop.stop_requested())
    return std::make_pair(Signals::STOP, Signals::STOP);

  std::pair<qsizetype, qsizetype> res = FindFinalResult();
  std::cout << "Out time is: " << Timer.StopTimer() << "ms\n";
  std::cout << "Num: " << res.first << " Count: " << res.second << "\n";

  return res;
}

std::pair<qsizetype, qsizetype> CollatzProcessorImpl::CalculateCollatz(
    qsizetype current_element) {
  qsizetype original_element = current_element;
  qsizetype step_counter = 0;
  while (current_element != 1) {
    if (current_element % 2) {
      current_element = current_element * 3 + 1;
      if (WillOverflow(current_element))
        return std::make_pair(OVERFLOW, OVERFLOW);
    } else {
      if ((current_element & (current_element - 1)) == 0) {
        return std::make_pair(original_element,
                              std::log2(current_element) + step_counter);
      }
      current_element /= 2;
    }
    ++step_counter;
  }
  return std::make_pair(original_element, step_counter);
}

void CollatzProcessorImpl::SaveFinalThreadResult(
    std::pair<qsizetype, qsizetype> final_thread_result) {
  std::lock_guard<std::mutex> write_access_lock{ThreadResultsLock};
  ThreadResults.push_back(final_thread_result);
  // for (auto p : ThreadResults) {
  //   std::cout << "Val: " << p.first << " Count: " << p.second << "\n";
  // }
}

std::pair<qsizetype, qsizetype> CollatzProcessorImpl::FindFinalResult() {
  auto result = *std::ranges::max_element(
      ThreadResults, [](const std::pair<qsizetype, qsizetype> first_el,
                        const std::pair<qsizetype, qsizetype> second_el) {
        return first_el.second < second_el.second;
      });
  ThreadResults.clear();
  return result;
}

void CollatzProcessorImpl::Run(std::stop_token stop,
                               const qsizetype CurrentUpperLimit) {
  // std::cout << "MAX: " << s_MaxSize << "\n";
  qsizetype current_element = Elements++;
  std::pair<qsizetype, qsizetype> local_thread_result{1, 0};

  std::pair<qsizetype, qsizetype> current_result;
  while (current_element <= CurrentUpperLimit) {
    current_result = CalculateCollatz(current_element);
    if (current_result.first == Signals::OVERFLOW &&
        current_result.second == Signals::OVERFLOW) {
      RequestStop();
    }
    if (stop.stop_requested()) return;
    if (current_result.second > local_thread_result.second)
      local_thread_result = current_result;

    current_element = Elements++;
  }
  SaveFinalThreadResult(local_thread_result);
}
}  // namespace impl
}  // namespace Core
