#pragma once
#include <QList>
#include <QThread>
#include <atomic>
#include <limits>
#include <thread>

namespace Core {
namespace impl {
class CollatzProcessorImpl {
 public:
  static const qsizetype s_CoresCount;
  static const QList<std::jthread> s_ThreadPool;
  static const qsizetype s_MaxSize;

  std::pair<int, int> StartProcessing(const qsizetype CurrentUpperLimit,
                                      const qsizetype CurrentThreadLimit);
};

}  // namespace impl
}  // namespace Core
