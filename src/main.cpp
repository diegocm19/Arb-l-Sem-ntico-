#include <QApplication>
#include "VentanaPrincipal.hpp"
int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  VentanaPrincipal w;
  w.show();
  return app.exec();
}