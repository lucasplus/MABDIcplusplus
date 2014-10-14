#include "SimGui.h"
#include "ui_SimGui.h"

#include <vtkRenderWindow.h>

#include "SimGuiSettings.h"

#include <QDir>
#include <QVariant>

SimGui::SimGui(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::MainWidget)
{
  std::cout << "SimGui::SimGui()" << std::endl;
  
  ui->setupUi(this);

  // set up the environment (add objects and set colors)
  environmentSetup();

  ui->scenarioViewVTKWindow->GetRenderWindow()->AddRenderer( sensor.renderer );

  connect( ui->objectListWidget, &QListWidget::itemChanged, 
    this, &SimGui::objectListChanged );
}

void SimGui::environmentSetup()
{
  std::cout << "SimGui::objectSetup() " << std::endl;

  // TODO: to make it so that the user can select a new environment while app is running:
  //    - need a clear method in both SimGuiSettings and MabdiSimulatedSensor

  QColor c;

  QString pathToEnvironment = settings.getSetting( SimGuiSettings::Key::EnvironmentDir ).toString();

  // find all files in the directory pathToObjects matching "*.stl"
  QDir dir( pathToEnvironment );
  if( !dir.exists() )
    qFatal("Cannot find directory with mesh files.");
  QStringList filter;
  filter << "*.stl";
  dir.setNameFilters( filter );
  QFileInfoList dirFileInfoList;
  dirFileInfoList = dir.entryInfoList( QDir::Files, QDir::Name );
  
  // add the objects to both the gui and MabdiSimulatedSensor
  for(int i=0; i<dirFileInfoList.size(); ++i){
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
  
  // It's ugly but I am making a 2nd for loop to set the default colors of the objects
  // Have to do this because of the way I index through the objects in MabdiSimulatedSensor
  // I should also be setting defaults in SimGuiSettings but I would need to know the number of objects
  QList<QColor> objectColorList;
  c = QColor( Qt::GlobalColor::blue );
  for(int i=0; i<dirFileInfoList.size(); ++i){
    sensor.setObjectColor( i, c.red(), c.green(), c.blue() );
    objectColorList.append( c );
  }

  // here we are saving the QList with QColor of each object into the config file
  // using the ObjectColor key. See SimGuiSettings
  QVariant v;
  v.setValue( objectColorList );
  settings.setSetting( SimGuiSettings::Key::ObjectColor, v );

  // background color
  c = settings.getSetting( SimGuiSettings::Key::ScenarioViewBackgroundColor ).value<QColor>();
  sensor.renderer->SetBackground( c.red(), c.green(), c.blue() );
}

void SimGui::objectListChanged( QListWidgetItem* item )
{
  std::cout << "SimGui::objectListChanged(): " 
    << item->text().toStdString() << std::endl;

  // which row of the QListWidget?
  int row;
  row = item->listWidget()->row( item );

  // set visible or hide
  Qt::CheckState state = item->checkState();
  if( state==Qt::CheckState::Checked )
    sensor.setObjectVisibility( row, true );
  else
    sensor.setObjectVisibility( row, false );

  // have to rerender to show changes
  ui->scenarioViewVTKWindow->GetRenderWindow()->Render();
}

SimGui::~SimGui()
{
  delete ui;
}
 
