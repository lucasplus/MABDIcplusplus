#include <iostream>

#include "SimGuiSettings.h"

#include <QApplication>
#include <QFileInfo>

#include <QDir>
#include <QStringList>

#include <QList>
#include <QColor>

#include "qdebug.h"

SimGuiSettings::SimGuiSettings()
{
  // get the base name of the executable and use it to name the configuration file
  // note: CMake should use this same file name to prefill the configuration file (if needed)
  //       It will sit in the directory of the executable 
  //       Name of the file: <executable_name>_config.ini

  QString appName = QFileInfo( QApplication::applicationFilePath() ).baseName();
  qDebug() << "Executable name: " << appName;

  QString configFilePath = QApplication::applicationDirPath() 
    + "/" + appName + "_config.ini";
  qDebug() << "Config file: " << configFilePath;
  
  // have to register types unkown to QtCore (its a QVariant thing)
  qRegisterMetaType< QList<QColor> >("QList<QColor>");
  qRegisterMetaTypeStreamOperators< QList<QColor> >("QList<QColor>");

  settings = new QSettings( configFilePath, QSettings::IniFormat );

  // mapping SimGuiSettings enum to the key actually used by QSettings
  keyMap[Key::EnvironmentDir]              = "environment/directory";
  keyMap[Key::ScenarioViewBackgroundColor] = "scenario/view/background_color";
  keyMap[Key::ObjectColor]                 = "environment/object_color";

  writeDefaults();

  QString fileName = settings->fileName();
  qDebug() << "Config file out: " << fileName;
}

void SimGuiSettings::writeDefaults()
{
  QString thisQKey;
  
  // relative directory path to the environment
  thisQKey = keyMap[Key::EnvironmentDir];
  if( !settings->contains( thisQKey ) ){
    QString thisDefault = "util/stl/environment/";
    settings->setValue( thisQKey, thisDefault );
    getDirectoryInfo();
    writeObjectColorDefault();
  }
  else{
    getDirectoryInfo();
  }

  // scenario background color
  thisQKey = keyMap[Key::ScenarioViewBackgroundColor];
  if( !settings->contains( thisQKey ) ){
    QColor thisDefault =  QColor( Qt::GlobalColor::lightGray );
    settings->setValue( thisQKey, thisDefault );
  }
}

void SimGuiSettings::writeObjectColorDefault()
{
  QList<QColor> objectColorList;
  QColor c = QColor( Qt::GlobalColor::blue );
  for(auto i : objectFilePath){
    objectColorList << c;
  }

  QVariant v;
  QString thisQKey = keyMap[Key::ObjectColor];
  v.setValue( objectColorList );
  settings->setValue( thisQKey, v );
}

void SimGuiSettings::getDirectoryInfo()
{
  QString pathToEnvironment = getSetting( Key::EnvironmentDir ).toString();
  
  // find all files in the directory pathToObjects matching "*.stl"
  QDir dir( pathToEnvironment );
  if( !dir.exists() )
    qFatal("Cannot find directory with mesh files.");
  QStringList filter;
  filter << "*.stl";
  dir.setNameFilters( filter );
  QFileInfoList dirFileInfoList;
  dirFileInfoList = dir.entryInfoList( QDir::Files, QDir::Name );

  // save the base name and the file path of the object
  QString baseName;
  objectBaseName.clear();
  objectFilePath.clear();
  for(auto file : dirFileInfoList){
    baseName = file.baseName();
    baseName.replace("_"," ");
    objectBaseName << baseName;
    objectFilePath << file.filePath();
  }

}

QList<QString> SimGuiSettings::getObjectBaseName()
{
  return objectBaseName;
}

QList<QString> SimGuiSettings::getObjectFilePath()
{
  return objectFilePath;
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

  if (inKey == Key::EnvironmentDir ){
    getDirectoryInfo();
    writeObjectColorDefault();
  }
}

SimGuiSettings::~SimGuiSettings()
{
  qDebug() << "SimGuiSettings destructor";
  delete settings;
}


