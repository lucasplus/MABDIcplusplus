#include <QApplication>
#include "MabdiSimulationGui.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  MabdiSimulationGui mabdiSimulationGui;
  mabdiSimulationGui.show();

  return app.exec();
}