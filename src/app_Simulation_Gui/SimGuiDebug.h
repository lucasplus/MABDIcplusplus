#ifndef SIMGUIDEBUG_H
#define SIMGUIDEBUG_H

#include <QApplication>

#include <QTextEdit>
 
// Class for printing debug information to the GUI's QTextEdit window
// Qt:
//   - Use qDebug() and qWarning() in the application source code to print messages 
//   - Messages from the class SimGuiSettings will only show in the console because it 
//     has to be initiated first
//   - You have to #include "qdebug.h" when you want to use the "<<" operator

class SimGuiDebug
{
 
public:
  SimGuiDebug( QTextEdit *textEdit );
  ~SimGuiDebug();
  
};

#endif // SIMGUI_H