#pragma once

#include <QCoreApplication>
#include <QDialog>
#include <QMouseEvent>
#include <QStringLiteral>
#include <QThread>
#include <Timer.hpp>

namespace Ui {

class Dialog;

class View : public QDialog {
  Q_OBJECT
 public:
  View(QWidget* parent = nullptr) : QDialog{parent} {}
  virtual void DisplayCollatzResult(
      std::pair<qsizetype, qsizetype> CollatzResult,
      timer::Timer Time) noexcept = 0;
  virtual void DisplayStopMessage() noexcept = 0;
  virtual void DisplayOverflowMessage() noexcept = 0;
 signals:
  void SendViewInfo(qsizetype CoresSelected, qsizetype UpperLimit);
  void on_btnStop_clicked();

 protected:
  virtual void on_btnStart_clicked() noexcept = 0;
  virtual void on_btnExit_clicked() noexcept = 0;
};

class MainDialog : public View {
  Q_OBJECT

 public:
  MainDialog(QWidget* parent = nullptr);
  ~MainDialog();

 private slots:
  void on_btnStart_clicked() noexcept override;
  void on_btnExit_clicked() noexcept override;

  void on_sliderThreadCountSelector_valueChanged(int value) const noexcept;

  void DisplayCollatzResult(std::pair<qsizetype, qsizetype> CollatzResult,
                            timer::Timer Timer) noexcept override;
  void DisplayStopMessage() noexcept override;
  void DisplayOverflowMessage() noexcept override;

 public slots:

 private:
  Dialog* m_ui;
};

}  // namespace Ui
