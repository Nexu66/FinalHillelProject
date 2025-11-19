#pragma once
#include <QDebug>
#include <QObject>

#include "coreImpl.hpp"
namespace Core {
class Model : public QObject {
  Q_OBJECT
 public:
  virtual void StartProcessing(std::stop_token stop,
                               qsizetype CurrentCoresSelected,
                               qsizetype CurrentUpperLimit) noexcept = 0;
 signals:
  void SendCollatzResult(qsizetype CollatzResult, timer::Timer Time);
  void SendStopMessage();
  void SendOverflowMessage();
  void RetryBtnStart();
};

class CollatzProcessor : public Model {
  Q_OBJECT
  impl::CollatzProcessorImpl impl;

 private:
  void StartProcessing(std::stop_token stop, qsizetype CurrentCoresSelected,
                       qsizetype CurrentUpperLimit) noexcept override;
};

}  // namespace Core
