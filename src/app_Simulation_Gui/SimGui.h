#ifndef SIMGUI_H
#define SIMGUI_H

#include <QWidget>

#include <QListWidgetItem>

#include "SimGuiSettings.h"

#include "MabdiSimulatedSensor.h"

namespace Ui {
  class MainWidget;
}
 
class SimGui : public QWidget
{
  Q_OBJECT
 
public:
  SimGui( QWidget *parent = 0 );
  ~SimGui();
  
private slots:
  void objectListChanged( QListWidgetItem* );

private:
  Ui::MainWidget *ui;
  SimGuiSettings settings;
  
  MabdiSimulatedSensor sensor;
  
  void environmentSetup();
};

#endif // SIMGUI_H