#ifndef SIMGUIDEBUG_H
#define SIMGUIDEBUG_H

#include "SimGuiSettings.h"

#include <QTextEdit>
 
class SimGuiDebug
{
  // Q_OBJECT
 
public:
  SimGuiDebug( QTextEdit *textEdit );
  ~SimGuiDebug();

private:
  QTextEdit* textEdit;
  
};

#endif // SIMGUI_H