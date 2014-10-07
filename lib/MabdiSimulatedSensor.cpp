
#include "MabdiSimulatedSensor.h"

#include <vtkWeakPointer.h>

#include <vtkSTLReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>

#include <vtkActorCollection.h>

#include <vtkProperty.h>

MabdiSimulatedSensor::MabdiSimulatedSensor(){
  std::cout << "MabdiSimulatedSensor::MabdiSimulatedSensor()" << std::endl;

  renderer = vtkSmartPointer<vtkRenderer>::New();
}

void MabdiSimulatedSensor::addObject( const char* newObjectPath ){
  std::cout << "MabdiSimulatedSensor::addObject()" << std::endl;

  vtkSmartPointer<vtkSTLReader> reader =
    vtkSmartPointer<vtkSTLReader>::New();
  vtkSmartPointer<vtkPolyDataMapper> polyMapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  vtkSmartPointer<vtkActor> actor =
    vtkSmartPointer<vtkActor>::New();

  reader->SetFileName( newObjectPath );
  polyMapper->SetInputConnection( reader->GetOutputPort() );
  polyMapper->Update();
  actor->SetMapper( polyMapper );
  
  renderer->AddActor( actor );
}

void MabdiSimulatedSensor::showObject( int row, bool showObject ){
  std::cout << "MabdiSimulatedSensor::showObject(): " << 
    "row number " << row << std::endl;

  // get the actors from the renderer
  // use a vtkWeakPointer because the renderer is the owner
  vtkWeakPointer<vtkActorCollection> actorCollection = renderer->GetActors();  
  
  // get the actor corresponding to the given row
  vtkWeakPointer<vtkActor> actor;
  actorCollection->InitTraversal();
  for( int i=0; i<=row; ++i ){
    std::cout << "i = " << i << std::endl;
    actor = actorCollection->GetNextActor();
  }
  
  // show or hide object
  if( showObject )
    actor->SetVisibility( 1 );
  else
    actor->SetVisibility( 0 );

  
  //renderer->Render();
}

  /*
  vtkProperty *p = vtkProperty::New();
  p->SetColor(255,0,0);
  p->SetOpacity(0.5);

  actorCollection->ApplyProperties( p );
  */