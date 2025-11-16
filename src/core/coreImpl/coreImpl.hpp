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
enum Signals { STOP = -1, OVERFLOW = -2 };
class CollatzProcessorImpl {
 public:
  bool is_Overflow = false;
  static const qsizetype s_CoresCount;
  static const qsizetype s_MaxSize = std::numeric_limits<qsizetype>::max();
  static std::atomic<qsizetype> Elements;
  std::vector<std::pair<qsizetype, qsizetype>> ThreadResults;
  std::mutex ThreadResultsLock;
  static std::vector<std::jthread> s_ThreadPool;
  timer::Timer Timer;

  void RequestStop();
  bool WillOverflow(qsizetype current_element);
  std::pair<qsizetype, qsizetype> StartProcessing(
      std::stop_token stop, const qsizetype CurrentThreadLimit,
      const qsizetype CurrentUpperLimit);
  std::pair<qsizetype, qsizetype> CalculateCollatz(qsizetype current_element);
  void SaveFinalThreadResult(
      std::pair<qsizetype, qsizetype> final_thread_result);
  std::pair<qsizetype, qsizetype> FindFinalResult();
  void Run(std::stop_token stop, const qsizetype CurrentUpperLimit);
};

}  // namespace impl
}  // namespace Core
