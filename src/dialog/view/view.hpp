#pragma once

#include <QCoreApplication>
#include <QDialog>
#include <QMouseEvent>
#include <QStringLiteral>
#include <QThread>

namespace Ui {

class View : public QDialog {
  Q_OBJECT
 public:
  View(QWidget* Parent = nullptr) : QDialog{Parent} {}
  virtual ~View() = default;
  virtual void DisplayCollatzResult(
      const std::pair<qsizetype, qsizetype> CollatzResult,
      const double Time) noexcept = 0;
  virtual void DisplayStopMessage() noexcept = 0;
  virtual void DisplayOverflowMessage() noexcept = 0;
  virtual void RetrySendingViewInfo() noexcept = 0;

 protected:
  virtual void on_btnStart_clicked() noexcept = 0;
  virtual void on_btnExit_clicked() noexcept = 0;
 signals:
  void SendViewInfo(const qsizetype CoresSelected, const qsizetype UpperLimit);
  void on_btnStop_clicked();
};
}  // namespace Ui
