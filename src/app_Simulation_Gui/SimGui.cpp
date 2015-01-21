#include "SimGui.h"
#include "ui_SimGui.h"

#include <vtkRenderWindow.h>

#include "SimGuiSettings.h"

#include <QList>
#include <QString>

#include <QVariant>

#include <QAction>

#include <QColorDialog>

SimGui::SimGui(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::MainWidget)
{
  std::cout << "SimGui::SimGui()" << std::endl;

  ui->setupUi(this);
  ui->infoTextEdit->setTextColor( QColor( Qt::GlobalColor::blue ) );
  ui->infoTextEdit->append( QString("hai") );

  // set up the environment (add objects and set colors)
  environmentSetup();

  ui->scenarioViewVTKWindow->GetRenderWindow()->AddRenderer( sensor.renderer );

  connect( ui->objectListWidget, &QListWidget::itemChanged, 
    this, &SimGui::objectListChanged );
}


// call when the user changes directory where *.stl files are saved
void SimGui::environmentSetup()
{
  std::cout << "SimGui::environmentSetup() " << std::endl;

  // get name and file path for each object
  QList<QString> objectBaseName = settings.getObjectBaseName();
  QList<QString> objectFilePath = settings.getObjectFilePath();

  //
  // QListWidgetItem - for toggling objects on and off
  //

  // clear and then add items to the object list 
  ui->objectListWidget->clear();
  ui->objectListWidget->addItems( QStringList(objectBaseName) );

  // make all items in the list checkable
  QList<QListWidgetItem*> itemList 
    = ui->objectListWidget->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard);
  for( auto item : itemList ){
    item->setFlags( Qt::ItemIsUserCheckable | Qt::ItemIsEnabled );
    item->setCheckState( Qt::CheckState::Checked );
  }

  //
  // QAction - for changing object color
  //

  QList<QAction*> actionList = ui->objectListWidget->actions();
  for( auto action : actionList ){
    ui->objectListWidget->removeAction( action );
  }

  QString contextMenuTitle;
  QAction *action;
  for( auto baseName : objectBaseName ){
    contextMenuTitle = "Change color: " + baseName;
    action = new QAction( contextMenuTitle, this );
    action->setObjectName( baseName );
    ui->objectListWidget->addAction( action );
    connect( action, &QAction::triggered, this, &SimGui::changeObjectColor );
  }

  // context menu item to change the color of the background
  action = new QAction( "Change color: background", this );
  action->setObjectName( "background" );
  ui->objectListWidget->addAction( action );
  connect( action, &QAction::triggered, this, &SimGui::changeObjectColor );

  //
  // MabdiSimulatedSensor - add object to the simulated sensor and set color
  //

  for(auto filePath : objectFilePath ){
    sensor.addObject( filePath.toStdString().c_str() );
  }

  QList<QColor> objectColorList 
    = settings.getSetting( SimGuiSettings::Key::ObjectColor ).value< QList<QColor> >();
  int count = 0;
  for( auto c : objectColorList ){
    sensor.setObjectColor( count, c.redF(), c.greenF(), c.blueF() );
    count++;
  }

  // background color
  QColor c = settings.getSetting( SimGuiSettings::Key::ScenarioViewBackgroundColor ).value<QColor>();
  sensor.setBackgroundColor( c.redF(), c.greenF(), c.blueF() );
}

void SimGui::changeObjectColor(){
  std::cout << "SimGui::changeObjectColor()" << std::endl;

  // get which action sent the signal and its objectName
  QAction* thisAction = qobject_cast<QAction*>( sender() );

  // is it the background?
  if( QString("background") == thisAction->objectName() ){
    QColor oldColor = 
      settings.getSetting( SimGuiSettings::Key::ScenarioViewBackgroundColor ).value< QColor >();
    QColor c = QColorDialog::getColor( oldColor );
    if ( !c.isValid() ) { 
      return;
    }
    sensor.setBackgroundColor( c.redF(), c.greenF(), c.blueF() );
    QVariant v;
    v.setValue( c );
    settings.setSetting( SimGuiSettings::Key::ScenarioViewBackgroundColor, v );
    return;
  }
  
  // list of all objects
  QList<QString> objectBaseName = settings.getObjectBaseName();

  // cycle through list, find index of object (by matching name)
  int objectIndex = 0; 
  for( auto i : objectBaseName ){
    std::cout << "i is: " << i.toStdString() << std::endl;
    if ( i == thisAction->objectName() ) {
      objectIndex = objectBaseName.indexOf( i );
    }
  }

  // get the old color list from the settings and change it for the new color
  // unless the color choosen was invalid (user probably cancelled the QColorDialog)
  QList<QColor> objectColorList 
    = settings.getSetting( SimGuiSettings::Key::ObjectColor ).value< QList<QColor> >();
  QColor c = QColorDialog::getColor( objectColorList[objectIndex] );
  if ( !c.isValid() ) {
    std::cout << "No color choosen" << std::endl; 
    return;
  }
  objectColorList[ objectIndex ] = c;
  
  // save the color list changes back into settings
  QVariant v;
  v.setValue( objectColorList );
  settings.setSetting( SimGuiSettings::Key::ObjectColor, v );

  // change in MabdiSimulatedSensor
  sensor.setObjectColor( objectIndex, c.redF(), c.greenF(), c.blueF() );

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
 
