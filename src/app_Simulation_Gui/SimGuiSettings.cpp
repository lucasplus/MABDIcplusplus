#include <iostream>

#include "SimGuiSettings.h"

#include <QApplication>


SimGuiSettings::SimGuiSettings()
{
  std::cout << "SimGuiSettings::SimGuiSettings()" << std::endl;
  QString configFilePath = QApplication::applicationDirPath() + "/config.ini";
  std::cout << "Config file: " << configFilePath.toStdString() << std::endl;
  
  settings = new QSettings( configFilePath, QSettings::IniFormat );
}
