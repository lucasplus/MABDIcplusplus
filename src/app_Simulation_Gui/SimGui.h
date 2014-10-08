#ifndef SIMGUI_H
#define SIMGUI_H

#include <QWidget>

#include <QListWidgetItem>

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
  std::vector<QListWidgetItem*> objects;
  MabdiSimulatedSensor sensor;

  Ui::MainWidget *ui;

  QString configFilePath;

  void objectSetup( const char* );
};

#endif // SIMGUI_H