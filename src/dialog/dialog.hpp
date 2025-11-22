#pragma once

#include "view.hpp"

namespace Ui {

class Dialog;

class MainDialog : public View {
  Q_OBJECT

 public:
  MainDialog(QWidget* Parent = nullptr);
  ~MainDialog();

 private slots:
  void on_btnStart_clicked() noexcept override;
  void on_btnExit_clicked() noexcept override;

  void on_sliderThreadCountSelector_valueChanged(
      const int Value) const noexcept;

  void DisplayCollatzResult(const std::pair<qsizetype, qsizetype> CollatzResult,
                            const double Time) noexcept override;
  void DisplayStopMessage() noexcept override;
  void DisplayOverflowMessage() noexcept override;
  void RetrySendingViewInfo() noexcept override;

 private:
  Dialog* m_Ui;
};

}  // namespace Ui
