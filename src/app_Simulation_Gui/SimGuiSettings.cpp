#include <iostream>

#include "SimGuiSettings.h"

#include <QApplication>
#include <QFileInfo>

#include <QList>
#include <QColor>


SimGuiSettings::SimGuiSettings()
{
  std::cout << "SimGuiSettings::SimGuiSettings()" << std::endl;
  
  // get the base name of the executable and use it to name the configuration file
  // note: CMake should use this same file name to prefill the configuration file
  //       It will sit in the directory of the executable 
  //       Name of the file: <executable_name>_config.ini

  QString appName = QFileInfo( QApplication::applicationFilePath() ).baseName();
  std::cout << "Executable name: " << appName.toStdString() << std::endl;

  QString configFilePath = QApplication::applicationDirPath() 
    + "/" + appName + "_config.ini";
  std::cout << "Config file: " << configFilePath.toStdString() << std::endl;
  
  qRegisterMetaType< QList<QColor> >("QList<QColor>");
  qRegisterMetaTypeStreamOperators< QList<QColor> >("QList<QColor>");

  settings = new QSettings( configFilePath, QSettings::IniFormat );

  // mapping SimGuiSettings enum to the key actually used by QSettings
  keyMap[Key::EnvironmentDir]              = "environment/directory";
  keyMap[Key::ScenarioViewBackgroundColor] = "scenario/view/background_color";
  keyMap[Key::ObjectColor]                 = "environment/object_color";

  writeDefaults();

  QString fileName = settings->fileName();
  std::cout << "Config file out: " << fileName.toStdString() << std::endl;
}

void SimGuiSettings::writeDefaults()
{
  QString thisQKey;
  
  // relative directory path to the environment
  thisQKey = keyMap[Key::EnvironmentDir];
  if( !settings->contains( thisQKey ) ){
    QString thisDefault = "util/stl/environment/";
    settings->setValue( thisQKey, thisDefault );
  }

  // scenario background color
  thisQKey = keyMap[Key::ScenarioViewBackgroundColor];
  if( !settings->contains( thisQKey ) ){
    QColor thisDefault =  QColor( Qt::GlobalColor::lightGray );
    settings->setValue( thisQKey, thisDefault );
  }
}

QVariant SimGuiSettings::getSetting( SimGuiSettings::Key inKey )
{
  QString thisQKey = keyMap[inKey];
  return settings->value( thisQKey );
}

void SimGuiSettings::setSetting( SimGuiSettings::Key inKey, QVariant inVariant )
{
  QString thisQKey = keyMap[inKey];
  settings->setValue( thisQKey, inVariant );
}

SimGuiSettings::~SimGuiSettings()
{
  std::cout << "SimGuiSettings::~SimGuiSettings()" << std::endl;
  delete settings;
}


