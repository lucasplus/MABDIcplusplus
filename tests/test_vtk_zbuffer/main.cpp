// This demo creates depth map for a polydata instance by extracting
// exact ZBuffer values.
#include <vtkSmartPointer.h>
 
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSTLReader.h>
#include <vtkBMPWriter.h>
#include <vtkWindowToImageFilter.h>
#include <vtkImageShiftScale.h>

#include <vtkImageMapper.h>
#include <vtkActor2D.h>
#include <vtkInteractorStyleImage.h>

#include "log.h"
#include "util.h"

_INITIALIZE_EASYLOGGINGPP
 
int main(int argc, char *argv[])
{
  // logging setup
  // LOG(INFO) and LOG(TRACE)
  el::Configurations conf( MABDI_UTIL_LOG_CONFIG_FILE );
  LOG(INFO) << std::endl <<"_____________________ starting";
  el::Loggers::reconfigureAllLoggers( conf );

  // read in ply files

  UTIL_VTK_INIT( vtkSTLReader             , fileReader       );
  UTIL_VTK_INIT( vtkPolyDataMapper        , mapper           );
  UTIL_VTK_INIT( vtkActor                 , actor            );
  UTIL_VTK_INIT( vtkRenderer              , rendererPoly     );
  UTIL_VTK_INIT( vtkRenderer              , rendererImage    );
  UTIL_VTK_INIT( vtkRenderWindow          , renWinPoly       );
  UTIL_VTK_INIT( vtkRenderWindow          , renWinImage      );

  UTIL_VTK_INIT( vtkRenderWindowInteractor, interactorPoly   );
  UTIL_VTK_INIT( vtkRenderWindowInteractor, interactorImage  );
  UTIL_VTK_INIT( vtkInteractorStyleImage  , style            );
  
  UTIL_VTK_INIT( vtkWindowToImageFilter   , filter           );
  UTIL_VTK_INIT( vtkImageMapper           , imageMapper      );
  UTIL_VTK_INIT( vtkImageShiftScale       , scale            );
  


  
  
  interactorImage->SetInteractorStyle( style );

  

  //vtkSmartPointer<vtkImageShiftScale> scale =
  //  vtkSmartPointer<vtkImageShiftScale>::New();
 
  // read .ply file
  fileReader->SetFileName("util/stl/environment/table.stl");
 
  // Build visualization enviroment
  mapper->SetInputConnection( fileReader->GetOutputPort() );
  actor->SetMapper(mapper);
  rendererPoly->AddActor(actor);
  renWinPoly->AddRenderer(rendererPoly);
  interactorPoly->SetRenderWindow(renWinPoly);
  renWinPoly->Render();
  interactorPoly->Start();
  
  // Create Depth Map
  filter->SetInput(renWinPoly);
  filter->SetMagnification(1);
  filter->SetInputBufferTypeToZBuffer();  // extract z buffer value
  
  

  scale->SetOutputScalarTypeToUnsignedChar();
  scale->SetInputConnection( filter->GetOutputPort() );
  scale->SetShift(0);
  scale->SetScale(-255);

  imageMapper->SetInputData( scale->GetOutput() );

  vtkSmartPointer<vtkActor2D> imageActor = 
    vtkSmartPointer<vtkActor2D>::New();

  imageActor->SetMapper( imageMapper );

  renWinImage->AddRenderer( rendererImage );

  

  interactorImage->SetRenderWindow( renWinImage );
  
  rendererImage->AddActor2D( imageActor );
  
  renWinImage->Render();
  interactorImage->Start();

  vtkImageData* myImage = filter->GetOutput();
  
  /*
  //scale->SetOutputScalarTypeToUnsignedChar();
  //scale->SetInputConnection(filter->GetOutputPort());
  //scale->SetShift(0);
  //scale->SetScale(-255);
  
  filter->GetOutputPort()

  // Write depth map as a .bmp image
  imageWriter->SetFileName("hai.bmp");
  imageWriter->SetInputConnection(scale->GetOutputPort());
  imageWriter->Write();
  */
  return EXIT_SUCCESS;
}