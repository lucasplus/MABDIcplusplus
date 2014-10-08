#ifndef SIMGUISETTINGS_H
#define SIMGUISETTINGS_H

#include <QSettings>

class SimGuiSettings
{
public:
  SimGuiSettings();
private:
  QSettings *settings;
};

#endif // SIMGUISETTINGS_H