#pragma once
#include <QDebug>
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
  static QList<std::jthread> s_ThreadPool;
  static const qsizetype s_MaxSize;
  static std::atomic<int> element;

  std::pair<int, int> StartProcessing(const qsizetype CurrentThreadLimit,
                                      const qsizetype CurrentUpperLimit);
};

}  // namespace impl
}  // namespace Core
