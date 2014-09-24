#include "MabdiSimulationGui.h"
#include "ui_MabdiSimulationGui.h"
 
MabdiSimulationGui::MabdiSimulationGui(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::MainWidget)
{
  ui->setupUi(this);
}
 
MabdiSimulationGui::~MabdiSimulationGui()
{
  delete ui;
}
 
