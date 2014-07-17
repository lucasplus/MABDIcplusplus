#include "gui.h"

#include <QtGui>
#include <QtWidgets> // QPushButton, QCheckBox
#include <QVTKWidget.h>
#include <QString>

#include <QSizePolicy>

#include <vtkPolyData.h>
#include <vtkPLYReader.h>
#include <vtkSmartPointer.h>

#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyle.h>

#include <vector>
#include <string>

#include <QDir>
#include <QString>
#include <QStringList>
#include <QFileInfoList>

// _________________________________________________________________________
Gui::Gui( QWidget *parent ) : QWidget(parent)
{

  setWindowTitle( "MABDI app: ply viewer" );

  // ___________________________________ find ply files

  QDir plyDir("util/ply/environment");
  if( !plyDir.exists() )
    qFatal("Cannot find directory with ply files.");

  QStringList filters;
  filters << "*.ply";
  plyDir.setNameFilters( filters );

  QFileInfoList plyFileInfoList;
  plyFileInfoList = plyDir.entryInfoList( QDir::Files, QDir::Name );

  int nPlyFiles;
  nPlyFiles = plyFileInfoList.size();

  std::cout << plyFileInfoList.size() << std::endl;
  for( int i=0; i<plyFileInfoList.size(); ++i ){
    std::cout << plyFileInfoList[i].filePath().toStdString() << std::endl;
  }

  // ___________________________________ initialize widgets

  std::vector< QCheckBox* > vecQCheckBox;
  for( int i=0; i<plyFileInfoList.size(); ++i ){
    vecQCheckBox.push_back( new QCheckBox(plyFileInfoList[i].baseName().toStdString().c_str()) );
  }

  goButton = new QPushButton( "go" );

  renderWindowWidget = new QVTKWidget( parent );
  renderWindowWidget->resize(256,256);

  // ___________________________________ arrange ui widgets

  QHBoxLayout *hLayout = new QHBoxLayout;
  QVBoxLayout *vLayout = new QVBoxLayout;

  vLayout->addWidget( goButton );
  std::vector< QCheckBox* >::iterator it;
  for( it=vecQCheckBox.begin(); it!=vecQCheckBox.end(); ++it ){
    vLayout->addWidget( *it );
  }
  hLayout->addLayout( vLayout );
  hLayout->addWidget( renderWindowWidget );
  
  setLayout( hLayout );
  
  // ___________________________________ reading and displaying poly data

  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  renderWindow->AddRenderer(renderer);

  // init and setup readers, mappers, actors
  std::vector< vtkSmartPointer<vtkPLYReader> > vecReader;
  std::vector< vtkSmartPointer<vtkPolyDataMapper> > vecMapper;
  std::vector< vtkSmartPointer<vtkActor> > vecActor;
  double r, g, b;
  for( int i=0; i<plyFileInfoList.size(); ++i ){
    // vector of readers, init and setup
    vecReader.push_back( vtkSmartPointer<vtkPLYReader>::New() );
    vecReader[i]->SetFileName( plyFileInfoList[i].filePath().toStdString().c_str() );
    // vector of mappers, init and setup
    vecMapper.push_back( vtkSmartPointer<vtkPolyDataMapper>::New() );
    vecMapper[i]->SetInputConnection( vecReader[i]->GetOutputPort() );
    // vector of actors, init and setup
    vecActor.push_back( vtkSmartPointer<vtkActor>::New() );
    vecActor[i]->SetMapper( vecMapper[i] );
    vecActor[i]->GetProperty()->SetColor(255,0,0);
    vecActor[i]->GetProperty()->SetOpacity(0.5);
    
    vecActor[i]->GetProperty()->GetColor(r,g,b);
    std::cout << " colors " << r << " " << g << " " << b << std::endl;
    // add actors to renderer
    renderer->AddActor( vecActor[i] );
  }
  renderer->SetBackground(238,232,170); 
  renderer->ResetCamera();

  renderWindowWidget->GetRenderWindow()->AddRenderer(renderer);
  //renderWindowInteractor->SetRenderWindow(renderWindowWidget->GetRenderWindow());
  renderWindowWidget->SetRenderWindow(renderWindow);
  renderWindowWidget->show();
}


