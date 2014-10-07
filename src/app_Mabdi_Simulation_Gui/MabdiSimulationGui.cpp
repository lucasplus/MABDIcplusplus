#include "MabdiSimulationGui.h"
#include "ui_MabdiSimulationGui.h"

#include <vtkRenderWindow.h>

#include "MABDI_Util.h"

#include <QDir>

MabdiSimulationGui::MabdiSimulationGui(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::MainWidget)
{
  ui->setupUi(this);
  
  // use settings
  objectSetup( MABDI_Util::StlEnvironmentDir.c_str() );

  ui->qvtkWidgetScenarioView->GetRenderWindow()->AddRenderer( sensor.renderer );

  std::vector<double> rgb( MABDI_Util::BackgroundRGB );
  sensor.renderer->SetBackground( rgb[0], rgb[1], rgb[2] );

  connect( ui->objectListWidget, &QListWidget::itemChanged, 
    this, &MabdiSimulationGui::objectListChanged );

}

void MabdiSimulationGui::objectSetup( const char* pathToObjects )
{
  std::cout << "MabdiSimulationGui::objectSetup() " << std::endl;

  // find all files in the directory pathToObjects matching "*.stl"
  QDir dir( pathToObjects );
  if( !dir.exists() )
    qFatal("Cannot find directory with mesh files.");
  QStringList filter;
  filter << "*.stl";
  dir.setNameFilters( filter );
  QFileInfoList dirFileInfoList;
  dirFileInfoList = dir.entryInfoList( QDir::Files, QDir::Name );
  
  // add the objects to both the gui and MabdiSimulatedSensor
  for(int i=0; i < dirFileInfoList.size(); ++i){
    // take out underscore and replace with space
    QString baseName = dirFileInfoList[i].baseName();
    baseName.replace("_"," ");

    // add to QListWidget and set checkable 
    ui->objectListWidget->addItem( baseName );
    QListWidgetItem* item = ui->objectListWidget->item( i );
    item->setFlags( Qt::ItemIsUserCheckable | Qt::ItemIsEnabled );
    item->setCheckState( Qt::CheckState::Checked );

    // add to MabdiSimulatedSensor object
    sensor.addObject( dirFileInfoList[i].filePath().toStdString().c_str() );
  }

}

void MabdiSimulationGui::objectListChanged( QListWidgetItem* item )
{
  std::cout << "MabdiSimulationGui::objectListChanged(): " 
    << item->text().toStdString() << std::endl;

  // which row of the QListWidget?
  int row;
  row = item->listWidget()->row( item );

  // set visible or hide
  Qt::CheckState state = item->checkState();
  if( state==Qt::CheckState::Checked )
    sensor.showObject( row, true );
  else
    sensor.showObject( row, false );

  // have to rerender to show changes
  ui->qvtkWidgetScenarioView->GetRenderWindow()->Render();
}

MabdiSimulationGui::~MabdiSimulationGui()
{
  delete ui;
}
 
