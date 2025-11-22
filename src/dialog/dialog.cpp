#include "dialog.hpp"

#include "ui_dialog.h"

namespace Ui {
MainDialog::MainDialog(QWidget* Parent) : View{Parent}, m_Ui{new Ui::Dialog} {
  m_Ui->setupUi(this);
  m_Ui->sliderThreadCountSelector->setMinimum(1);
  m_Ui->sliderThreadCountSelector->setMaximum(QThread::idealThreadCount());
  m_Ui->sliderThreadCountSelector->setTickInterval(1);
}

MainDialog::~MainDialog() { delete m_Ui; }

void MainDialog::on_btnStart_clicked() noexcept {
  m_Ui->btnStart->setEnabled(false);
  m_Ui->leResult->clear();
  m_Ui->leTimeResult->clear();
  emit this->SendViewInfo(m_Ui->sliderThreadCountSelector->value(),
                          m_Ui->spinBoxNumRange->value());
}

void MainDialog::on_btnExit_clicked() noexcept { this->close(); }

void MainDialog::on_sliderThreadCountSelector_valueChanged(
    const int Value) const noexcept {
  m_Ui->leThreads->setText(QString::number(Value));
}
void MainDialog::DisplayCollatzResult(
    const std::pair<qsizetype, qsizetype> CollatzResult,
    const double Time) noexcept {
  m_Ui->leResult->setText(
      QStringLiteral("Number: ") + QString::number(CollatzResult.first) +
      QStringLiteral(" Lenght: ") + QString::number(CollatzResult.second));
  m_Ui->leTimeResult->setText(QString::number(Time) + QStringLiteral("ms"));
  m_Ui->btnStart->setEnabled(true);
}

void MainDialog::DisplayStopMessage() noexcept {
  m_Ui->leResult->setText(QStringLiteral("Stop issued!"));
  m_Ui->leTimeResult->setText(QStringLiteral("xxxxx.ms"));
  m_Ui->btnStart->setEnabled(true);
}

void MainDialog::DisplayOverflowMessage() noexcept {
  m_Ui->leResult->setText(QStringLiteral("OVERFLOW!!!"));
  m_Ui->leTimeResult->setText(QStringLiteral("xxxxx.ms"));
  m_Ui->btnStart->setEnabled(true);
}

void MainDialog::RetrySendingViewInfo() noexcept {
  emit this->SendViewInfo(m_Ui->sliderThreadCountSelector->value(),
                          m_Ui->spinBoxNumRange->value());
}

}  // namespace Ui
