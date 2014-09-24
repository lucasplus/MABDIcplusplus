#ifndef MABDISIMULATIONGUI_H
#define MABDISIMULATIONGUI_H
 
#include <QWidget>
 
namespace Ui {
  class MainWidget;
}
 
class MabdiSimulationGui : public QWidget
{
  Q_OBJECT
 
public:
  explicit MabdiSimulationGui(QWidget *parent = 0);
  ~MabdiSimulationGui();
 
private:
  Ui::MainWidget *ui;
};

#endif // MABDISIMULATIONGUI_H