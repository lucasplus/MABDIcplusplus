#ifndef MABDISCENARIOSIMULATEDSENSOR_H
#define MABDISCENARIOSIMULATEDSENSOR_H

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

class MabdiScenarioSimulatedSensor
{
public:
  MabdiScenarioSimulatedSensor ();


}

#endif // MABDISCENARIOSIMULATEDSENSOR_H