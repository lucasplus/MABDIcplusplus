
#include "SimGuiDebug.h"

#include <QFileInfo>
#include <QString>

#include "stdlib.h"

#include "qdebug.h"

// qInstallMessageHandler has an error if the handler function is not in an anonymous namespace
namespace{
  
  QTextEdit* textEdit;  // need this pointer for use in the message handler
  
  void messageHandlerQt(QtMsgType type, const QMessageLogContext & context, const QString & msg)
  {
    //
    // assemble the message
    //
    
    QString filePath = QString( context.file );
    QString file = QFileInfo( filePath ).fileName();

    int line = context.line;

    QString bigMessage;
    bigMessage.append("[ ");
    bigMessage.append(file);
    bigMessage.append(" : ");
    bigMessage.append( QString::number( line ) );
    bigMessage.append(" : ");
    bigMessage.append( QString(context.function) );
    bigMessage.append(" ] \n  ");
    bigMessage.append(msg);

    //
    // change the color of the message based on the type 
    //

    switch (type) {
    case QtDebugMsg:
      textEdit->setTextColor( QColor( Qt::GlobalColor::darkGreen ) );
      textEdit->append( bigMessage );
      textEdit->setTextColor( QColor( Qt::GlobalColor::black ) );
      break;
    case QtWarningMsg:
      textEdit->setTextColor( QColor( Qt::GlobalColor::red ) );
      textEdit->append( msg );
      textEdit->setTextColor( QColor( Qt::GlobalColor::black ) );
      break;
    case QtCriticalMsg:
      textEdit->append( msg );
      break;
    case QtFatalMsg:
      textEdit->append( msg );
      abort();
    }
  }

}

SimGuiDebug::SimGuiDebug( QTextEdit* inTextEdit )
{
  textEdit = inTextEdit;  // pointer to the QTextEdit box
  
  textEdit->setFontPointSize( 9 );  // make text a bit bigger 
  
  qInstallMessageHandler( messageHandlerQt );  // handler for qt messages
}

SimGuiDebug::~SimGuiDebug()
{
}