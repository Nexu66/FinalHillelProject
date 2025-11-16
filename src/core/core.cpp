#include "core.hpp"
namespace Core {
void CollatzProcessor::StartProcessing(std::stop_token stop,
                                       qsizetype CurrentThreadLimit,
                                       qsizetype CurrentUpperLimit) noexcept {
  qInfo() << "tl:" << CurrentThreadLimit << "ul:" << CurrentUpperLimit;
  auto algorithm_result =
      impl.StartProcessing(stop, CurrentThreadLimit, CurrentUpperLimit);
  if (algorithm_result.first == impl::Signals::STOP &&
      algorithm_result.second == impl::STOP)
    emit SendStopMessage();
  else if (algorithm_result.first == impl::Signals::OVERFLOW &&
           algorithm_result.second == impl::Signals::OVERFLOW) {
    emit SendOverflowMessage();
  } else {
    emit SendCollatzResult(algorithm_result, impl.Timer);
  }
}

}  // namespace Core
