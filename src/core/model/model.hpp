#pragma once
#include <QDebug>
#include <QObject>
#include <stop_token>

#include "Timer.hpp"

namespace Core {
class Model : public QObject {
  Q_OBJECT
 public:
  virtual void StartProcessing(const std::stop_token StopToken,
                               const qsizetype CurrentCoresSelected,
                               const qsizetype CurrentUpperLimit) noexcept = 0;
 signals:
  void SendCollatzResult(const std::pair<qsizetype, qsizetype> CollatzResult,
                         const double Time);
  void SendStopMessage();
  void SendOverflowMessage();
  void RetryBtnStart();
};
}  // namespace Core
