#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include "canvaswidget.h"
#include "compositeselection.h"

class MainWindow : public QMainWindow
{

   Q_OBJECT

   public:
  
      MainWindow(ICamera* camera);

   private:
      ICamera* camera;
      CanvasWidget* canvas;
      QPushButton* button;   
      QDockWidget* leftDock;
      shared_ptr<CompositePaths> compositePaths;
      CompositeSelection* compositeSelection;

};

#endif // WINDOW_H