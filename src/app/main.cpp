#include <QApplication>

#include "controller.hpp"
#include "core.hpp"
#include "dialog.hpp"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  Ctrl::Controller c{new Ui::MainDialog{}, new Core::CollatzProcessor{}};
  c.show();
  return a.exec();
}
