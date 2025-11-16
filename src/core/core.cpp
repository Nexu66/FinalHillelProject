#include "core.hpp"
namespace Core {
void CollatzProcessor::StartProcessing(qsizetype CurrentThreadLimit,
                                       qsizetype CurrentUpperLimit) noexcept {
  qInfo() << "tl:" << CurrentThreadLimit << "ul:" << CurrentUpperLimit;
  impl.StartProcessing(CurrentThreadLimit, CurrentUpperLimit);
}
}  // namespace Core
