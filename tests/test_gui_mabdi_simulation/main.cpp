#include <QApplication>
#include "MabdiSimulationGui.h"

int main(int argc, char *argv[])
{
  // Q_INIT_RESOURCE(textfinder);
  QApplication app(argc, argv);

  MabdiSimulationGui mabdiSimulationGui;
  mabdiSimulationGui.show();

  return app.exec();
}