#pragma once

#include <QCoreApplication>
#include <QDialog>
#include <QThread>

namespace Ui {
class Dialog;

class MainDialog : public QDialog {
  Q_OBJECT

 public:
  MainDialog(QWidget* parent = nullptr);
  ~MainDialog();

 private slots:
  void on_btnExit_clicked() noexcept;

  void on_sliderThreadCountSelector_valueChanged(int value) const noexcept;

 private:
  Dialog* m_ui;
  const unsigned int m_totalCores = QThread::idealThreadCount();
};

}  // namespace Ui
