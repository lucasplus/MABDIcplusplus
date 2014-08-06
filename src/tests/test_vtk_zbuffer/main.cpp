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
 
int main(int argc, char *argv[])
{
  vtkSmartPointer<vtkPLYReader> fileReader = 
    vtkSmartPointer<vtkPLYReader>::New();
  
  vtkSmartPointer<vtkPolyDataMapper> mapper = 
    vtkSmartPointer<vtkPolyDataMapper>::New();
  
  vtkSmartPointer<vtkActor> actor = 
    vtkSmartPointer<vtkActor>::New();
  
  vtkSmartPointer<vtkRenderer> rendererPoly = 
    vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderer> rendererDepth = 
    vtkSmartPointer<vtkRenderer>::New();

  vtkSmartPointer<vtkRenderWindow> renWinPoly = 
    vtkSmartPointer<vtkRenderWindow>::New();
  vtkSmartPointer<vtkRenderWindow> renWinDepth = 
    vtkSmartPointer<vtkRenderWindow>::New();
  
  vtkSmartPointer<vtkRenderWindowInteractor> interactor = 
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  
  vtkSmartPointer<vtkWindowToImageFilter> filter =
    vtkSmartPointer<vtkWindowToImageFilter>::New();

  //vtkSmartPointer<vtkImageShiftScale> scale =
  //  vtkSmartPointer<vtkImageShiftScale>::New();
 
  // Read .vtp file
  fileReader->SetFileName("util/ply/environment/table.ply");
 
  // Build visualization enviroment
  mapper->SetInputConnection( fileReader->GetOutputPort() );
  actor->SetMapper(mapper);
  rendererPoly->AddActor(actor);
  renWinPoly->AddRenderer(rendererPoly);
  interactor->SetRenderWindow(renWinPoly);
  renWinPoly->Render();
  
  /*
  // Create Depth Map
  filter->SetInput(renWinPoly);
  filter->SetMagnification(1);
  filter->SetInputBufferTypeToZBuffer();        //Extract z buffer value
 
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