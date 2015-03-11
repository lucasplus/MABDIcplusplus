#include <iostream>
#include "MabdiDebug.h"

#include <regex>

INITIALIZE_EASYLOGGINGPP

MabdiDebug::MabdiDebug()
{
  // configure easylogging so that nothing is written to a file or the terminal
  el::Configurations defaultConf;
  defaultConf.setToDefault();
  defaultConf.set(el::Level::Info,el::ConfigurationType::ToStandardOutput,"false");
  defaultConf.set(el::Level::Info,el::ConfigurationType::ToFile,"false");
  el::Loggers::reconfigureLogger("default",defaultConf);
}

void MabdiDebug::handle( const el::LogDispatchData* data )
{
  const el::LogMessage* logMessage = data->logMessage();
  
  // get the file name
  std::string filePath = logMessage->file();
  std::regex fileRegEx("\\w*.\\w*$",std::regex_constants::ECMAScript);
  std::smatch match;
  std::regex_search( filePath, match, fileRegEx );
  std::string fileName = match[0];
  
  // assemble message
  std::string bigMessage;
  bigMessage.append("[ ");
  bigMessage.append( fileName );
  bigMessage.append(" : ");
  bigMessage.append( std::to_string( logMessage->line() ) );
  bigMessage.append(" : ");
  bigMessage.append( logMessage->func() );
  bigMessage.append(" ] \n  -- ");
  bigMessage.append( logMessage->message() );

  // use the installed handle to send
  installedHandle( bigMessage );
}

void MabdiDebug::installMessageHandleFunction( std::function<void(std::string)> inHandle )
{
  installedHandle = inHandle;
}

