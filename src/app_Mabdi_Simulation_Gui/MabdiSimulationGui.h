#ifndef MABDISIMULATIONGUI_H
#define MABDISIMULATIONGUI_H

#include <QWidget>

#include <QListWidgetItem>

#include "MabdiSimulatedSensor.h"

namespace Ui {
  class MainWidget;
}
 
class MabdiSimulationGui : public QWidget
{
  Q_OBJECT
 
public:
  MabdiSimulationGui( QWidget *parent = 0 );
  ~MabdiSimulationGui();
  
private slots:
  void objectListChanged( QListWidgetItem* );

private:
  std::vector<QListWidgetItem*> objects;
  MabdiSimulatedSensor sensor;

  Ui::MainWidget *ui;

  void objectSetup( const char* );
};

#endif // MABDISIMULATIONGUI_H