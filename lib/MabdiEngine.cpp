
#include "MabdiEngine.h"

#include "MabdiDebug.h"
#include "EasyLogging.h"

MabdiEngine::MabdiEngine()
{
  
}

void MabdiEngine::initializeDebugMessages( std::function<void(std::string)> inHandle ){
  el::Helpers::installLogDispatchCallback<MabdiDebug>("MabdiDebug");
  MabdiDebug* mabdiDebug = el::Helpers::logDispatchCallback<MabdiDebug>("MabdiDebug");
  mabdiDebug->installMessageHandleFunction( inHandle );
  LOG(INFO) << "MABDI message handle function installed.";
}

