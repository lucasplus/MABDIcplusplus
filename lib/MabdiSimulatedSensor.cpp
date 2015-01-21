
#include "MabdiSimulatedSensor.h"

#include <vtkNew.h>
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

  vtkNew<vtkSTLReader> reader;
  vtkNew<vtkPolyDataMapper> polyMapper;
  vtkNew<vtkActor> actor;

  reader->SetFileName( newObjectPath );
  polyMapper->SetInputConnection( reader->GetOutputPort() );
  polyMapper->Update();

  std::cout << "polymapper ScalarVisibility: " << polyMapper->GetScalarVisibility() << std::endl;
  
  actor->SetMapper( polyMapper.GetPointer() );
  
  renderer->AddActor( actor.GetPointer() );
}

void MabdiSimulatedSensor::setObjectVisibility( int row, bool showObject ){
  std::cout << "MabdiSimulatedSensor::setObjectVisibility(): " << 
    "row number " << row << std::endl;

  vtkWeakPointer<vtkActor> actor = getActor( row );
  
  // show or hide object
  if( showObject )
    actor->SetVisibility( 1 );
  else
    actor->SetVisibility( 0 );
}

void MabdiSimulatedSensor::setObjectColor( int row, double r, double g, double b ){
  std::cout << "MabdiSimulatedSensor::setObjectColor(): " << 
    "row number " << row << std::endl;

  vtkWeakPointer<vtkActor> actor = getActor( row );
  
  std::cout << "color: " << r << " " << g << " " << b << " " << std::endl; 

  actor->GetProperty()->SetColor( r, g, b );
}

void MabdiSimulatedSensor::setBackgroundColor( double r, double g, double b ){
  renderer->SetBackground( r, g, b );
}

vtkActor* MabdiSimulatedSensor::getActor( int row ){
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

  return actor;
}