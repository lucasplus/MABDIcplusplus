#ifndef SIMGUISETTINGS_H
#define SIMGUISETTINGS_H

#include <QSettings>
#include <QVariant>
#include <QMap>

// SimGuiSettings 
//
// Class to capture settings that we wish to be persistant for the user
// each time the application is opened. e.g. Background colors and directory paths
// 
//  scenario
//    environment
//      directory - Key::EnvironmentDir
//      object
//        color - Key::ObjectColor
//    view 
//      background color - Key::ScenarioViewBackgroundColor

class SimGuiSettings
{
public:
  SimGuiSettings();
  ~SimGuiSettings();
  
  // enum containing keys to the settings that this class contains
  enum Key
  {
    EnvironmentDir,               // QString
    ObjectColor,                  // QList<QColor>
    ScenarioViewBackgroundColor,  // QColor
  };

  QVariant getSetting( Key );
  void setSetting( Key, QVariant );
  
private:
  QSettings *settings;
  QMap<Key, QString> keyMap;

  void writeDefaults();
};

#endif // SIMGUISETTINGS_H