#ifndef MABDIDEBUG_H
#define MABDIDEBUG_H

#include "EasyLogging.h"

#include <string>

#include <functional>

/*
Class to interface with EasyLogging.h
  Extends the class el::LogDispatchCallback of EasyLogging.h
    Allows me to catch messages sent using LOG(INFO)
  Applications using MABDI can install a message handler using installMessageHandleFunction
    The application gives MABDI a callable function that will be called anytime LOG(INFO) is used in MABDI
    The application has to implement the callable function.
*/

class MabdiDebug : public el::LogDispatchCallback
{
public:
  MabdiDebug();

  // extended from base class el::LogDispatchCallback
  void handle( const el::LogDispatchData* );
  
  // save a callable function from the user's application program
  // call the function when LOG(INFO) is used
  void installMessageHandleFunction( std::function<void(std::string)> );

private:
  std::function<void(std::string)> installedHandle;
};

#endif // MABDIDEBUG_H