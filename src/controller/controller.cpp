#include "controller.hpp"

namespace Ctrl {
Controller::Controller(Ui::UserInterface* ui, Core::Model* mod)
    : m_ui{ui}, m_mod{mod} {
  QObject::connect(m_ui.get(), &Ui::UserInterface::SendViewInfo, m_mod.get(),
                   &Core::Model::StartProcessing);
}

void Controller::show() const noexcept { m_ui->show(); }

void Controller::Start(qsizetype Threads, qsizetype Limit) {
  qInfo() << "T:" << Threads << "L:" << Limit;
}
}  // namespace Ctrl
