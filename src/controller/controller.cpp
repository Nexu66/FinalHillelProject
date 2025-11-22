#include "controller.hpp"

namespace Ctrl {

Controller::Controller(Ui::View* pView, Core::Model* pModel)
    : m_pView{pView}, m_pModel{pModel} {
  QObject::connect(m_pView.get(), &Ui::View::SendViewInfo, this,
                   &Controller::LaunchModel);
  QObject::connect(m_pModel.get(), &Core::Model::SendCollatzResult,
                   m_pView.get(), &Ui::View::DisplayCollatzResult);
  QObject::connect(m_pModel.get(), &Core::Model::SendStopMessage, m_pView.get(),
                   &Ui::View::DisplayStopMessage);
  QObject::connect(m_pModel.get(), &Core::Model::SendOverflowMessage,
                   m_pView.get(), &Ui::View::DisplayOverflowMessage);
  QObject::connect(m_pModel.get(), &Core::Model::RetryBtnStart, m_pView.get(),
                   &Ui::View::RetrySendingViewInfo);
  QObject::connect(m_pView.get(), &Ui::View::on_btnStop_clicked, this,
                   &Controller::StopAlgorithm);
}

void Controller::LaunchModel(const qsizetype CurrentCoresSelected,
                             const qsizetype CurrentUpperLimit) {
  m_ModelThread = std::jthread{[&](std::stop_token StopToken) {
    m_pModel->StartProcessing(StopToken, CurrentCoresSelected,
                              CurrentUpperLimit);
  }};
}

void Controller::StopAlgorithm() { m_ModelThread.request_stop(); }

void Controller::show() const noexcept { m_pView->show(); }

}  // namespace Ctrl
