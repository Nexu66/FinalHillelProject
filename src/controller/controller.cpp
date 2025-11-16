#include "controller.hpp"

namespace Ctrl {

Controller::Controller(Ui::View* ui, Core::Model* mod) : m_ui{ui}, m_mod{mod} {
  QObject::connect(m_ui.get(), &Ui::View::SendViewInfo, this,
                   &Controller::LaunchModel);
  QObject::connect(m_mod.get(), &Core::Model::SendCollatzResult, m_ui.get(),
                   &Ui::View::DisplayCollatzResult);
  QObject::connect(m_mod.get(), &Core::Model::SendStopMessage, m_ui.get(),
                   &Ui::View::DisplayStopMessage);
  QObject::connect(m_mod.get(), &Core::Model::SendOverflowMessage, m_ui.get(),
                   &Ui::View::DisplayOverflowMessage);
  QObject::connect(m_ui.get(), &Ui::View::on_btnStop_clicked, this,
                   &Controller::StopAlgorithm);
}

void Controller::LaunchModel(qsizetype CurrentCoresSelected,
                             qsizetype CurrentUpperLimit) {
  ModelThread = std::jthread{[&](std::stop_token stop) {
    m_mod->StartProcessing(stop, CurrentCoresSelected, CurrentUpperLimit);
  }};
}

void Controller::StopAlgorithm() { ModelThread.request_stop(); }

void Controller::show() const noexcept { m_ui->show(); }

}  // namespace Ctrl
