#include <QApplication>

#include "dialog.hpp"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  Ui::MainDialog w;
  w.show();
  return a.exec();
}
