#ifndef MABDIENGINE_H
#define MABDIENGINE_H

#include <string>
#include <iostream>
#include <sstream>

#include <functional>

#include "MabdiSimulatedSensor.h"

/*
Class to run the MABDI Engine. Meaning, the implementation of the MABDI algorithm
*/

class MabdiEngine
{
public:
  MabdiEngine();
  MabdiSimulatedSensor sensor;
  
  // User application can provide MABDI a callable function that will be 
  // called when debug messages are written. See MabdiDebug.h
  void initializeDebugMessages( std::function<void(std::string)> );

};

#endif // MABDIENGINE_H