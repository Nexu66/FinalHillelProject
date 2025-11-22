#include "core.hpp"
namespace Core {
void CollatzProcessor::StartProcessing(
    const std::stop_token StopToken, const qsizetype CurrentThreadLimit,
    const qsizetype CurrentUpperLimit) noexcept {
  if (CurrentThreadLimit >= 1 &&
      CurrentThreadLimit <= impl::CollatzProcessorImpl::cs_CoresCount &&
      CurrentUpperLimit >= 1 && CurrentUpperLimit <= 1000000000) {
    auto algorithm_result = m_Impl.StartProcessing(
        StopToken, CurrentThreadLimit, CurrentUpperLimit);
    if (algorithm_result.first == impl::Signals::STOP &&
        algorithm_result.second == impl::Signals::STOP)
      emit SendStopMessage();
    else if (algorithm_result.first == impl::Signals::VALUE_OVERFLOWED &&
             algorithm_result.second == impl::Signals::VALUE_OVERFLOWED) {
      emit SendOverflowMessage();
    } else {
      emit SendCollatzResult(algorithm_result, m_Impl.s_Timer.time);
    }
  } else {
    emit RetryBtnStart();
  }
}

}  // namespace Core
