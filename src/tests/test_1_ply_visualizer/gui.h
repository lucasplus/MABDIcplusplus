
#ifndef GUI_H
#define GUI_H

#include <QWidget>
#include <QtWidgets>

#include <QString>
#include <QSizePolicy>
#include <QDir>
#include <QStringList>
#include <QFileInfoList>

#include <QVTKWidget.h>

#include <vtkPolyData.h>
#include <vtkPLYReader.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyle.h>

#include <vector>
#include <string>

class Gui : public QWidget
{
  Q_OBJECT

  public:
    Gui( QWidget *parent = 0 );
  
  private:
    std::vector< vtkSmartPointer<vtkPLYReader> > vecReader;
    std::vector< vtkSmartPointer<vtkPolyDataMapper> > vecMapper;
    std::vector< vtkSmartPointer<vtkActor> > vecActor;
    QTextEdit* statusText;
    QPushButton* goButton;
    QVTKWidget* renderWindowWidget;
};

#endif // GUI_H
