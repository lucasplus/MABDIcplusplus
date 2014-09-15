// This demo creates depth map for a polydata instance by extracting
// exact ZBuffer values.
#include <vtkSmartPointer.h>

#include <vtkSTLReader.h>
#include <vtkPolyDataMapper.h>

#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

#include <vtkWindowToImageFilter.h>
#include <vtkImageShiftScale.h>

#include <vtkImageMapper.h>
#include <vtkActor2D.h>
#include <vtkInteractorStyleImage.h>

#include <string>
#include <vector>

#include "MABDI_Util.h"
 
int main(int argc, char *argv[])
{

  // init vtk classes
  UTIL_VTK_INIT( vtkSTLReader             , fileReader       );
  UTIL_VTK_INIT( vtkPolyDataMapper        , mapper           );
  UTIL_VTK_INIT( vtkActor                 , actor            );
  UTIL_VTK_INIT( vtkRenderer              , renderer         );
  UTIL_VTK_INIT( vtkRenderWindow          , renderWindow     );
  UTIL_VTK_INIT( vtkRenderWindowInteractor, interactor       );
  UTIL_VTK_INIT( vtkWindowToImageFilter   , filter           );
  UTIL_VTK_INIT( vtkImageMapper           , imageMapper      );
  UTIL_VTK_INIT( vtkActor2D               , imageActor       );
  UTIL_VTK_INIT( vtkRenderWindow          , renderWindowImage);
  UTIL_VTK_INIT( vtkRenderer              , rendererImage    );

  // window and renderer set-up
  renderWindow->AddRenderer( renderer );
  renderWindowImage->AddRenderer( rendererImage );
  interactor->SetRenderWindow( renderWindow );
  
  // where is the file to read in?
  std::string tableFilePath( MABDI_Util::StlEnvironmentDir ); 
  tableFilePath.append( "table.stl" );

  // Data Pipeline [1] - read 
  fileReader->SetFileName( tableFilePath.c_str() );

  // Data Pipeline [2] - map to graphics primitive
  mapper->SetInputConnection( fileReader->GetOutputPort() );

  // Data Pipeline [3] - create an actor
  actor->SetMapper( mapper );

  // Add the actor to the renderer
  renderer->AddActor( actor );

  std::vector<double> rgb( MABDI_Util::BackgroundRGB );
  renderer->SetBackground( rgb[0], rgb[1], rgb[2] );

  renderWindow->Render();
  interactor->Start();

  filter->SetInput( renderWindow );
  filter->SetInput(renderWindow);
  filter->SetMagnification(2);
  filter->Update();

  // copy the output
  vtkImageData * outputData = filter->GetOutput()->NewInstance();
  outputData->DeepCopy(filter->GetOutput());

  imageMapper->SetInputData(outputData);
  imageMapper->SetColorWindow(255);
  imageMapper->SetColorLevel(127.5);

  imageActor->SetMapper( imageMapper );

  //renWin->SetSize(320, 320);
  //renWin->SetPosition(320,320);

  rendererImage->AddActor(imageActor);

  renderWindow->Render();
  renderWindowImage->Render();

  return EXIT_SUCCESS;
}

/*
// initialize vtk classes

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
