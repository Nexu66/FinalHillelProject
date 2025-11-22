#pragma once
#include <QDebug>
#include <QList>
#include <QThread>
#include <atomic>
#include <limits>
#include <ranges>
#include <thread>
#include <vector>

#include "Timer.hpp"

namespace Core {
namespace impl {
enum Signals { STOP = -1, VALUE_OVERFLOWED = -2 };
class CollatzProcessorImpl {
 public:
  CollatzProcessorImpl();
  ~CollatzProcessorImpl();
  bool IsOverflow = false;
  static const qsizetype cs_UpperLimitCap = 1000000000;
  static const qsizetype cs_CoresCount;
  static const qsizetype cs_MaxSizeBeforeOverflow =
      std::numeric_limits<qsizetype>::max() / 3 + 1;
  static std::vector<std::pair<qsizetype, qsizetype>> s_ThreadResults;
  static timer::Timer s_Timer;
  static std::atomic<qsizetype>* s_pCache;
  static std::vector<std::jthread> s_ThreadPool;

  void RequestStop();
  bool WillOverflow(const qsizetype CurrentElement);
  std::pair<qsizetype, qsizetype> StartProcessing(
      const std::stop_token StopToken, const qsizetype CurrentThreadLimit,
      const qsizetype CurrentUpperLimit);
  void CalculateCollatz(qsizetype CurrentElement, qsizetype& StepCounter);
  std::pair<qsizetype, qsizetype> FindFinalResult();
  void Run(const std::stop_token StopToken, const qsizetype IntervalBegin,
           const qsizetype IntervalEnd, const qsizetype IndexInResultsVector);
};

}  // namespace impl
}  // namespace Core
