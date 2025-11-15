#include "dialog.hpp"

#include "ui_dialog.h"

Ui::MainDialog::MainDialog(QWidget* parent)
    : QDialog{parent}, m_ui{new Ui::Dialog} {
  m_ui->setupUi(this);
  m_ui->sliderThreadCountSelector->setMinimum(1);
  m_ui->sliderThreadCountSelector->setMaximum(m_totalCores);
  m_ui->sliderThreadCountSelector->setTickPosition(QSlider::TicksLeft);
  m_ui->sliderThreadCountSelector->setTickInterval(m_totalCores);
}

Ui::MainDialog::~MainDialog() { delete m_ui; }

void Ui::MainDialog::on_btnExit_clicked() noexcept { this->close(); }

void Ui::MainDialog::on_sliderThreadCountSelector_valueChanged(
    int value) const noexcept {
  m_ui->lnCurrentThreadCount->setText(QString::number(value));
}
