#include "SimGui.h"
#include "ui_SimGui.h"

#include <vtkRenderWindow.h>

#include "SimGuiDebug.h"
#include "SimGuiSettings.h"

#include <QList>
#include <QString>
#include <QVariant>

#include <QAction>

#include <QColorDialog>

#include <functional>
#include <string>

#include "qdebug.h"

SimGui::SimGui(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::MainWidget)
{
  //
  // initialize
  //
  
  // create the gui. A Qt thing
  ui->setupUi(this);
  
  // class that will handle Qt and MABDI debug messages
  // it displays the messages in the application
  SimGuiDebug guiDebug( ui->infoTextEdit );
  engine.initializeDebugMessages( guiDebug.getMessageHandleFunction() );

  // find or make settings file, set defaults
  settings = new SimGuiSettings; 

  // set up the environment: add objects and set colors
  environmentSetup(); 

  engine.sensor.setup( 
    ui->scenarioViewVTKWindow->GetRenderWindow() , 
    ui->enlargedViewVTKWindow->GetRenderWindow() );

  connect( ui->objectListWidget, &QListWidget::itemChanged, 
    this, &SimGui::objectListChanged );

}

// call when the user changes directory where *.stl files are saved
void SimGui::environmentSetup()
{
  // get name and file path for each object
  QList<QString> objectBaseName = settings->getObjectBaseName();
  QList<QString> objectFilePath = settings->getObjectFilePath();

  qDebug() << "File paths to objects: " << objectFilePath;

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
    engine.sensor.addObject( filePath.toStdString().c_str() );
  }

  QList<QColor> objectColorList 
    = settings->getSetting( SimGuiSettings::Key::ObjectColor ).value< QList<QColor> >();
  int count = 0;
  for( auto c : objectColorList ){
    engine.sensor.setObjectColor( count, c.redF(), c.greenF(), c.blueF() );
    count++;
  }

  // background color
  QColor c = settings->getSetting( SimGuiSettings::Key::ScenarioViewBackgroundColor ).value<QColor>();
  engine.sensor.setBackgroundColor( c.redF(), c.greenF(), c.blueF() );
}

void SimGui::changeObjectColor(){

  // get which action sent the signal and its objectName
  QAction* thisAction = qobject_cast<QAction*>( sender() );

  qDebug() << "Change color of object: " << thisAction->objectName();

  // is it the background?
  if( QString("background") == thisAction->objectName() ){
    QColor oldColor = 
      settings->getSetting( SimGuiSettings::Key::ScenarioViewBackgroundColor ).value< QColor >();
    QColor c = QColorDialog::getColor( oldColor );
    if ( !c.isValid() ) { 
      return;
    }
    engine.sensor.setBackgroundColor( c.redF(), c.greenF(), c.blueF() );
    QVariant v;
    v.setValue( c );
    settings->setSetting( SimGuiSettings::Key::ScenarioViewBackgroundColor, v );
    return;
  }
  
  // list of all objects
  QList<QString> objectBaseName = settings->getObjectBaseName();

  // cycle through list, find index of object (by matching name)
  int objectIndex = 0; 
  for( auto i : objectBaseName ){
    if ( i == thisAction->objectName() ) {
      objectIndex = objectBaseName.indexOf( i );
    }
  }

  // get the old color list from the settings and change it for the new color
  // unless the color choosen was invalid (user probably cancelled the QColorDialog)
  QList<QColor> objectColorList 
    = settings->getSetting( SimGuiSettings::Key::ObjectColor ).value< QList<QColor> >();
  QColor c = QColorDialog::getColor( objectColorList[objectIndex] );
  if ( !c.isValid() ) {
    qWarning() << "No color choosen"; 
    return;
  }
  objectColorList.replace( objectIndex, c );
  
  // save the color list changes back into settings
  QVariant v;
  v.setValue( objectColorList );
  settings->setSetting( SimGuiSettings::Key::ObjectColor, v );

  // change in MabdiSimulatedSensor
  engine.sensor.setObjectColor( objectIndex, c.redF(), c.greenF(), c.blueF() );

}

void SimGui::objectListChanged( QListWidgetItem* item )
{
  // which row of the QListWidget?
  int row = item->listWidget()->row( item );

  

  // was it visible or was it hidden
  Qt::CheckState state = item->checkState();
  
  qDebug() << "Visibility state of object in row (zero based index): " << row 
    << " New state: " << ( state==Qt::CheckState::Checked );
  
  // set visible or hide
  if( state==Qt::CheckState::Checked )
    engine.sensor.setObjectVisibility( row, true );
  else
    engine.sensor.setObjectVisibility( row, false );

  // have to rerender to show changes
  ui->scenarioViewVTKWindow->GetRenderWindow()->Render();
}

SimGui::~SimGui()
{
  delete ui;
  delete settings;
}
 
