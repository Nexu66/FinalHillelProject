#include "dialog.hpp"

#include "ui_dialog.h"

namespace Ui {
MainDialog::MainDialog(QWidget* parent) : View{parent}, m_ui{new Ui::Dialog} {
  m_ui->setupUi(this);
  m_ui->sliderThreadCountSelector->setMinimum(1);
  m_ui->sliderThreadCountSelector->setMaximum(QThread::idealThreadCount());
  m_ui->sliderThreadCountSelector->setTickInterval(1);
}

MainDialog::~MainDialog() { delete m_ui; }

void MainDialog::on_btnStart_clicked() noexcept {
  m_ui->btnStart->setEnabled(false);
  m_ui->leResult->clear();
  m_ui->leTimeResult->clear();
  emit this->SendViewInfo(m_ui->sliderThreadCountSelector->value(),
                          m_ui->spinBoxNumRange->value());
}

void MainDialog::on_btnExit_clicked() noexcept { this->close(); }

void MainDialog::on_sliderThreadCountSelector_valueChanged(
    int value) const noexcept {
  m_ui->leThreads->setText(QString::number(value));
}
void MainDialog::DisplayCollatzResult(
    std::pair<qsizetype, qsizetype> CollatzResult, timer::Timer Time) noexcept {
  m_ui->leResult->setText(
      QStringLiteral("Number: ") + QString::number(CollatzResult.first) +
      QStringLiteral(" Lenght: ") + QString::number(CollatzResult.second));
  m_ui->leTimeResult->setText(QString::number(Time.time) +
                              QStringLiteral("ms"));
  m_ui->btnStart->setEnabled(true);
}

void MainDialog::DisplayStopMessage() noexcept {
  m_ui->leResult->setText(QStringLiteral("Stop issued!"));
  m_ui->leTimeResult->setText(QStringLiteral("xxxxx.ms"));
  m_ui->btnStart->setEnabled(true);
}

void MainDialog::DisplayOverflowMessage() noexcept {
  m_ui->leResult->setText(QStringLiteral("OVERFLOW!!!"));
  m_ui->leTimeResult->setText(QStringLiteral("xxxxx.ms"));
  m_ui->btnStart->setEnabled(true);
}

}  // namespace Ui
