#include "gui.h"

// _________________________________________________________________________
Gui::Gui( QWidget *parent ) : QWidget(parent)
{

  setWindowTitle( "MABDI app: mesh viewer" );

  // ___________________________________ find mesh files

  QDir meshDir("util/stl/environment");
  if( !meshDir.exists() )
    qFatal("Cannot find directory with mesh files.");

  QStringList filters;
  filters << "*.stl";
  meshDir.setNameFilters( filters );

  QFileInfoList meshFileInfoList;
  meshFileInfoList = meshDir.entryInfoList( QDir::Files, QDir::Name );

  int nMeshFiles;
  nMeshFiles = meshFileInfoList.size();

  std::cout << meshFileInfoList.size() << std::endl;
  for( int i=0; i<meshFileInfoList.size(); ++i ){
    std::cout << meshFileInfoList[i].filePath().toStdString() << std::endl;
  }

  // ___________________________________ initialize widgets

  std::vector< QCheckBox* > vecQCheckBox;
  for( int i=0; i<meshFileInfoList.size(); ++i ){
    vecQCheckBox.push_back( new QCheckBox(meshFileInfoList[i].baseName().toStdString().c_str()) );
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
  double r, g, b;
  for( int i=0; i<meshFileInfoList.size(); ++i ){
    // vector of readers, init and setup
    vecReader.push_back( vtkSmartPointer<vtkSTLReader>::New() );
    vecReader[i]->SetFileName( meshFileInfoList[i].filePath().toStdString().c_str() );
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
  renderWindowWidget->SetRenderWindow(renderWindow);
  renderWindowWidget->show();
}


