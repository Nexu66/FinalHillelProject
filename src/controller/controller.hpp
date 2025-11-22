#pragma once
#include <QAbstractButton>
#include <QScopedPointer>
#include <thread>

#include "model.hpp"
#include "view.hpp"
namespace Ctrl {
class Controller : public QObject {
  Q_OBJECT
  QScopedPointer<Ui::View> m_pView;
  QScopedPointer<Core::Model> m_pModel;
  std::jthread m_ModelThread;

 private slots:
  void LaunchModel(qsizetype CurrentCoresSelected, qsizetype CurrentUpperLimit);
  void StopAlgorithm();

 public:
  Controller(Ui::View* pView, Core::Model* pModel);
  void show() const noexcept;

};
}  // namespace Ctrl
