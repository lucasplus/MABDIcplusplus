// This demo creates depth map for a polydata instance by extracting
// exact ZBuffer values.
#include <vtkSmartPointer.h>
 
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPLYReader.h>
#include <vtkBMPWriter.h>
#include <vtkWindowToImageFilter.h>
#include <vtkImageShiftScale.h>

#include <vtkImageMapper.h>
#include <vtkActor2D.h>
#include <vtkInteractorStyleImage.h>
 
int main(int argc, char *argv[])
{
  // read in ply files
  vtkSmartPointer<vtkPLYReader> fileReader = 
    vtkSmartPointer<vtkPLYReader>::New();
  
  vtkSmartPointer<vtkPolyDataMapper> mapper = 
    vtkSmartPointer<vtkPolyDataMapper>::New();
  
  vtkSmartPointer<vtkActor> actor = 
    vtkSmartPointer<vtkActor>::New();
  
  vtkSmartPointer<vtkRenderer> rendererPoly = 
    vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderer> rendererImage = 
    vtkSmartPointer<vtkRenderer>::New();

  vtkSmartPointer<vtkRenderWindow> renWinPoly = 
    vtkSmartPointer<vtkRenderWindow>::New();
  vtkSmartPointer<vtkRenderWindow> renWinImage = 
    vtkSmartPointer<vtkRenderWindow>::New();
  
  vtkSmartPointer<vtkRenderWindowInteractor> interactorPoly = 
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  vtkSmartPointer<vtkRenderWindowInteractor> interactorImage = 
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  vtkSmartPointer<vtkInteractorStyleImage> style = 
    vtkSmartPointer<vtkInteractorStyleImage>::New();
  interactorImage->SetInteractorStyle( style );

  vtkSmartPointer<vtkWindowToImageFilter> filter =
    vtkSmartPointer<vtkWindowToImageFilter>::New();

  vtkSmartPointer<vtkImageMapper> imageMapper =
    vtkSmartPointer<vtkImageMapper>::New();

  vtkSmartPointer<vtkImageShiftScale> scale =
    vtkSmartPointer<vtkImageShiftScale>::New();

  //vtkSmartPointer<vtkImageShiftScale> scale =
  //  vtkSmartPointer<vtkImageShiftScale>::New();
 
  // read .ply file
  fileReader->SetFileName("util/ply/environment/table.ply");
 
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