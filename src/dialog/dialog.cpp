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
  emit this->SendViewInfo(m_ui->sliderThreadCountSelector->value(),
                          m_ui->spinBoxNumRange->value());
}

void MainDialog::on_btnExit_clicked() noexcept { this->close(); }

void MainDialog::on_sliderThreadCountSelector_valueChanged(
    int value) const noexcept {
  m_ui->leThreads->setText(QString::number(value));
}

}  // namespace Ui
