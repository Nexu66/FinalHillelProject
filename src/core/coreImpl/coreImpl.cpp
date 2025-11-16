#include "coreImpl.hpp"

namespace Core {
namespace impl {

const qsizetype CollatzProcessorImpl::s_CoresCount =
    QThread::idealThreadCount();
QList<std::jthread> CollatzProcessorImpl::s_ThreadPool{
    CollatzProcessorImpl::s_CoresCount};
const qsizetype s_MaxSize = std::numeric_limits<qsizetype>::max();
std::atomic<int> CollatzProcessorImpl::element = 0;

std::pair<int, int> CollatzProcessorImpl::StartProcessing(
    const qsizetype CurrentUpperLimit, const qsizetype CurrentThreadLimit) {
  // for (int i = 0; i < CurrentThreadLimit; ++i) {
  //   s_ThreadPool[i] = std::jthread{[&]() {
  //     // idk how to organize this
  //   }};
  qInfo() << "Hellow";
  return std::make_pair(1, 0);
}
}  // namespace impl
}  // namespace Core
