
#ifndef GUI_H
#define GUI_H

#include <QWidget>

#include <vtkRenderWindow.h>

#include <QVTKWidget.h>

// forward declaration
class QTextEdit;
class QPushButton;

class Gui : public QWidget
{
  Q_OBJECT

  public:
    Gui( QWidget *parent = 0 );
  
  private:
    QTextEdit* statusText;
    QPushButton* goButton;
    QVTKWidget* renderWindowWidget;
};

#endif // GUI_H
