#include "coreImpl.hpp"

namespace Core {
namespace impl {
const qsizetype CollatzProcessorImpl::s_CoresCount =
    QThread::idealThreadCount();
const QList<std::jthread> CollatzProcessorImpl::s_ThreadPool{
    CollatzProcessorImpl::CoresCount};
const qsizetype s_MaxSize = std::numeric_limits<qsizetype>::max();
std::atomic<int> CollatzProcessorImpl::element = 0;

std::pair<int, int> CollatzProcessorImpl::StartProcessing(
    const qsizetype CurrentUpperLimit, const qsizetype CurrentThreadLimit) {}
for (int i = 0; i < CurrentThreadLimit; ++i) {
}
}  // namespace impl
}  // namespace Core
