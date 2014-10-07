#include <QApplication>

#include "gui.h"

#include <iostream>

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  Gui theGui;
  theGui.show();

  return app.exec();
}
