#pragma once
#include <QDebug>
#include <QList>
#include <QThread>
#include <Timer.hpp>
#include <atomic>
#include <iostream>
#include <limits>
#include <mutex>
#include <ranges>
#include <semaphore>
#include <thread>
#include <vector>

namespace Core {
namespace impl {
struct ResultWrapper {
  alignas(64) std::pair<qsizetype, qsizetype> result{0, 0};
};
enum Signals { STOP = -1, VALUE_OVERFLOWED = -2 };
class CollatzProcessorImpl {
 public:
  CollatzProcessorImpl();
  ~CollatzProcessorImpl();
  bool is_Overflow = false;
  static const qsizetype cs_UpperLimitCap = 1000000000;
  static const qsizetype cs_CoresCount;
  static const qsizetype cs_MaxSizeBeforeOverflow =
      std::numeric_limits<qsizetype>::max() / 3 + 1;
  static std::vector<ResultWrapper> s_ThreadResults;
  static timer::Timer s_Timer;
  static std::atomic<qsizetype>* s_Cache;
  static std::vector<std::jthread> s_ThreadPool;

  void RequestStop();
  bool WillOverflow(qsizetype current_element);
  std::pair<qsizetype, qsizetype> StartProcessing(
      std::stop_token stop, const qsizetype CurrentThreadLimit,
      const qsizetype CurrentUpperLimit);
  qsizetype CalculateCollatz(qsizetype current_element, qsizetype step_counter);
  std::pair<qsizetype, qsizetype> FindFinalResult();
  void Run(std::stop_token stop, const qsizetype CurrentUpperLimit,
           const qsizetype IndexInResultsVector);
};

}  // namespace impl
}  // namespace Core
