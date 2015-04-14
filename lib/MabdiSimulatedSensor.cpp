
#include "MabdiSimulatedSensor.h"

#include <vtkNew.h>
#include <vtkWeakPointer.h>

#include <vtkSTLReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkActorCollection.h>

#include <vtkProperty.h>

#include <vtkCommand.h>
#include <vtkWindowToImageFilter.h>
#include <vtkImageMapper.h>
#include <vtkActor2D.h>

#include <vtkRenderWindow.h>
#include <vtkCamera.h>

#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyle.h>

class MyCallbackClass : public vtkCommand
{
public:
  static MyCallbackClass *New()
    {
    return new MyCallbackClass;
    }
  virtual void Execute(vtkObject *caller, unsigned long, void*)
    {
      double camPosition[3];
      double clipRange[2];
      double viewAngle;

      this->FilterSource->Modified();
      this->RenderWindowSource->Render();
      
      std::cout << "rendered event " << std::endl;
      
      this->RenderWindowCamera->GetPosition( camPosition );
      std::cout << "cam position " 
        << camPosition[0] << " " << camPosition[1] << " " << camPosition[2] << std::endl;
      
      this->RenderWindowCamera->GetClippingRange( clipRange );
      viewAngle = this->RenderWindowCamera->GetViewAngle();

      std::cout << " cam intrinsics " 
        << " np: " << clipRange[0] << " fp: " << clipRange[1] << " fovy: " << viewAngle << std::endl;

    }
  MyCallbackClass(): 
    FilterSource(0), 
    RenderWindowSource(0), 
    RenderWindowCamera(0) {}
  vtkWindowToImageFilter *FilterSource;
  vtkRenderWindow *RenderWindowSource;
  vtkCamera *RenderWindowCamera;
};


MabdiSimulatedSensor::MabdiSimulatedSensor(){
  renderer = vtkSmartPointer<vtkRenderer>::New(); // for the scenario

  rendererDepth = vtkSmartPointer<vtkRenderer>::New(); // for the sensor
}

void MabdiSimulatedSensor::setup( vtkRenderWindow* windowScene, vtkRenderWindow* windowDepth ){

  windowScene->AddRenderer( renderer );
  windowDepth->AddRenderer( rendererDepth );

  vtkSmartPointer<vtkWindowToImageFilter> filter = vtkSmartPointer<vtkWindowToImageFilter>::New();
  filter->SetInput( renderer->GetVTKWindow() );
  filter->SetInputBufferTypeToZBuffer();

  vtkSmartPointer<vtkImageMapper> imageMapper = vtkSmartPointer<vtkImageMapper>::New();
  imageMapper->SetInputConnection( filter->GetOutputPort() );
  imageMapper->SetColorWindow( 1 );
  imageMapper->SetColorLevel( 0.5 );

  vtkSmartPointer<vtkActor2D> imageActor = vtkSmartPointer<vtkActor2D>::New();
  imageActor->SetMapper( imageMapper );

  rendererDepth->AddActor( imageActor );

  vtkSmartPointer<MyCallbackClass> myCallback = vtkSmartPointer<MyCallbackClass>::New();
  myCallback->FilterSource = filter;
  myCallback->RenderWindowSource = rendererDepth->GetRenderWindow();
  myCallback->RenderWindowCamera = renderer->GetActiveCamera();

  renderer->GetActiveCamera()->SetViewAngle( 60.0 );
  renderer->GetActiveCamera()->SetClippingRange( 0.5, 10.0 );
  
  vtkSmartPointer<vtkInteractorStyle> style = 
    static_cast<vtkInteractorStyle *>(renderer->GetRenderWindow()->GetInteractor()->GetInteractorStyle());
  style->SetAutoAdjustCameraClippingRange( 0 );

  renderer->GetRenderWindow()->AddObserver( vtkCommand::RenderEvent, myCallback );
}

void MabdiSimulatedSensor::addObject( const char* newObjectPath ){
  LOG(INFO) << "Adding object with path: " << newObjectPath;

  vtkNew<vtkSTLReader> reader;
  vtkNew<vtkPolyDataMapper> polyMapper;
  vtkNew<vtkActor> actor;

  reader->SetFileName( newObjectPath );
  polyMapper->SetInputConnection( reader->GetOutputPort() );
  polyMapper->Update();
  
  actor->SetMapper( polyMapper.GetPointer() );
  
  renderer->AddActor( actor.GetPointer() );
}

void MabdiSimulatedSensor::setObjectVisibility( int row, bool showObject ){
  LOG(INFO) << "Setting object visibility of object in row:  " << row;

  vtkWeakPointer<vtkActor> actor = getActor( row );
  
  // show or hide object
  if( showObject )
    actor->SetVisibility( 1 );
  else
    actor->SetVisibility( 0 );
}

void MabdiSimulatedSensor::setObjectColor( int row, double r, double g, double b ){
  LOG(INFO) << "Setting object color of object in row:  " << row 
    << " Color: " << r << " " << g << " " << b;

  vtkWeakPointer<vtkActor> actor = getActor( row );

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
    actor = actorCollection->GetNextActor();
  }

  return actor;
}