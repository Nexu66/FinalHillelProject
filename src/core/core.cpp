#include "core.hpp"
namespace Core {
void CollatzProcessor::StartProcessing(std::stop_token stop,
                                       qsizetype CurrentThreadLimit,
                                       qsizetype CurrentUpperLimit) noexcept {
  qInfo() << "THREADS:" << CurrentThreadLimit << "LIMIT:" << CurrentUpperLimit;
  if (CurrentThreadLimit >= 1 &&
      CurrentThreadLimit <= impl::CollatzProcessorImpl::cs_CoresCount &&
      CurrentUpperLimit >= 1 && CurrentUpperLimit <= 1000000000) {
    auto algorithm_result =
        impl.StartProcessing(stop, CurrentThreadLimit, CurrentUpperLimit);
    if (algorithm_result.first == impl::Signals::STOP &&
        algorithm_result.second == impl::Signals::STOP)
      emit SendStopMessage();
    else if (algorithm_result.first == impl::Signals::VALUE_OVERFLOWED &&
             algorithm_result.second == impl::Signals::VALUE_OVERFLOWED) {
      emit SendOverflowMessage();
    } else {
      emit SendCollatzResult(algorithm_result, impl.s_Timer);
    }
  } else {
    emit RetryBtnStart();
  }
}

}  // namespace Core
