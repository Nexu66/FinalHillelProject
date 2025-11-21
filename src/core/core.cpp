#include "core.hpp"
namespace Core {
void CollatzProcessor::StartProcessing(std::stop_token stop,
                                       qsizetype CurrentThreadLimit,
                                       qsizetype CurrentUpperLimit) noexcept {
  qInfo() << "THREADS:" << CurrentThreadLimit << "LIMIT:" << CurrentUpperLimit;
  if (CurrentThreadLimit >= 1 &&
      CurrentThreadLimit <= impl::CollatzProcessorImpl::s_CoresCount) {
    auto algorithm_result =
        impl.StartProcessing(stop, CurrentThreadLimit, CurrentUpperLimit);
    if (algorithm_result.first == impl::Signals::STOP &&
        algorithm_result.second == impl::Signals::STOP)
      emit SendStopMessage();
    else if (algorithm_result.first == impl::Signals::VALUE_OVERFLOWED &&
             algorithm_result.second == impl::Signals::VALUE_OVERFLOWED) {
      emit SendOverflowMessage();
    } else {
      emit SendCollatzResult(algorithm_result, impl.Timer);
    }
  } else {
    emit RetryBtnStart();
  }
}

}  // namespace Core
