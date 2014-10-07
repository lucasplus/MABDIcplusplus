// This demo creates depth map for a polydata instance by extracting
// exact ZBuffer values.
#include <vtkSmartPointer.h>

#include <vtkSTLReader.h>
#include <vtkPolyDataMapper.h>

#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTerrain.h>

#include <vtkWindowToImageFilter.h>

#include <vtkImageMapper.h>
#include <vtkActor2D.h>
#include <vtkInteractorStyleImage.h>

#include <vtkCallbackCommand.h>

#include <vtkCamera.h>

#include <vtkDirectory.h>

#include <string>
#include <vector>

// a quicker way to initialize a vtkSmartPointer
#define UTIL_VTK_INIT( myClass, myObj ) \
  vtkSmartPointer<myClass> myObj = vtkSmartPointer<myClass>::New()

// The callback does the work.
// The callback keeps a pointer to the sphere whose resolution is
// controlled. After constructing the callback, the program sets the
// SphereSource of the callback to
// the object to be controlled.
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

int main(int argc, char *argv[])
{

  UTIL_VTK_INIT( vtkSTLReader               , fileReader            );
  UTIL_VTK_INIT( vtkPolyDataMapper          , mapper                );
  UTIL_VTK_INIT( vtkActor                   , actor                 );
  UTIL_VTK_INIT( vtkRenderer                , renderer              );
  UTIL_VTK_INIT( vtkRenderWindow            , renderWindow          );
  UTIL_VTK_INIT( vtkRenderWindowInteractor  , interactor            );
  UTIL_VTK_INIT( vtkInteractorStyleTerrain  , styleTerrain          );

  UTIL_VTK_INIT( vtkWindowToImageFilter     , filter                );
  UTIL_VTK_INIT( vtkImageMapper             , imageMapper           );
  UTIL_VTK_INIT( vtkActor2D                 , imageActor            );
  UTIL_VTK_INIT( vtkRenderWindow            , renderWindowImage     );
  UTIL_VTK_INIT( vtkRenderer                , rendererImage         );
  UTIL_VTK_INIT( MyCallbackClass            , myCallback            );

  UTIL_VTK_INIT( vtkCamera                  , ActiveCamera          );

  // window and renderer set-up
  renderWindow->AddRenderer( renderer );
  renderWindowImage->AddRenderer( rendererImage );
  interactor->SetRenderWindow( renderWindow );
  
  renderWindow->SetSize( 640, 480 );
  renderWindowImage->SetSize( 640, 480 );

  //renderWindow->SetOffScreenRendering( 1 );

  // Data Pipeline [1] - read 
  fileReader->SetFileName( "util/stl/environment/table.stl" );

  // Data Pipeline [2] - map to graphics primitive
  mapper->SetInputConnection( fileReader->GetOutputPort() );

  // Data Pipeline [3] - create an actor
  actor->SetMapper( mapper );

  // Add the actor to the renderer
  renderer->AddActor( actor );

  renderer->SetBackground( 238, 232, 170 );

  renderWindow->Render();

  filter->SetInput( renderWindow );
  filter->SetInputBufferTypeToZBuffer();

  imageMapper->SetInputConnection( filter->GetOutputPort() );
  imageMapper->SetColorWindow( 1 );
  imageMapper->SetColorLevel( 0.5 );

  imageActor->SetMapper( imageMapper );

  rendererImage->AddActor( imageActor );

  ActiveCamera = renderer->GetActiveCamera();

  renderer->Print( std::cout );

  // set up callback for when the renderwindow renders
  myCallback->FilterSource = filter;
  myCallback->RenderWindowSource = renderWindowImage;
  myCallback->RenderWindowCamera = ActiveCamera;
  renderWindow->AddObserver( vtkCommand::RenderEvent, myCallback );

  //styleTerrain->SetAutoAdjustCameraClippingRange( 0 );

  interactor->SetInteractorStyle( styleTerrain );

  ActiveCamera->SetViewAngle( 60.0 );
  ActiveCamera->SetClippingRange( 0.5, 10.0 );

  int isParallelProjection = ActiveCamera->GetParallelProjection();
  std::cout << " cam parallel projection " << isParallelProjection << std::endl;

  interactor->Start();
  
  return EXIT_SUCCESS;
}

