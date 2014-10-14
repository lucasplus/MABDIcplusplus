#ifndef MABDISIMULATEDSENSOR_H
#define MABDISIMULATEDSENSOR_H

#include <vtkSmartPointer.h>

#include <vtkActor.h>
#include <vtkActorCollection.h>

#include <vtkRenderer.h>

//#include "MABDI_Util.h"

/*

This is meant to class is meant to simulate a kinect sensor
by making use of the VTK library. 

This class is a subclass of MabdiScenarioSensor and so must implement
getImage();
getPose();

Needs to also give control of the camera and rendering pipeline. 
Maybe just hand out a pointer to the camera?
  Then the Gui class will be responsible for the vtk calls. 

*/

class MabdiSimulatedSensor
{
public:
  MabdiSimulatedSensor ();
  void addObject( const char* );
  void setObjectVisibility( int row, bool showObject );
  void setObjectColor( int row, int r, int g, int b );
  vtkSmartPointer<vtkRenderer> renderer;

private:
  vtkActor* getActor( int row );  
};

#endif // MABDISIMULATEDSENSOR_H