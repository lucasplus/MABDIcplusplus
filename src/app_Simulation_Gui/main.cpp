#include <iostream>
#include <QApplication>
#include "SimGui.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  SimGui simGui;
  simGui.show();

  return app.exec();
}