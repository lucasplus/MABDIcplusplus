#ifndef MABDISIMULATEDSENSOR_H
#define MABDISIMULATEDSENSOR_H

#include "EasyLogging.h"

#include <vtkSmartPointer.h>

#include <vtkActor.h>
#include <vtkActorCollection.h>

#include <vtkRenderer.h>

/*

Class to simulate a depth sensor (kinect)

*/

class MabdiSimulatedSensor
{
public:
  MabdiSimulatedSensor();
  void addObject( const char* );
  void setObjectVisibility( int row, bool showObject );
  void setObjectColor( int row, double r, double g, double b );
  void setBackgroundColor( double r, double g, double b );
  vtkSmartPointer<vtkRenderer> renderer;

private:
  vtkActor* getActor( int row );  
};

#endif // MABDISIMULATEDSENSOR_H