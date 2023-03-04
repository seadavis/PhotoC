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
      CanvasWidget* canvas;
      QPushButton* button;   
      QDockWidget* leftDock;
      CompositeSelection* compositeSelection;

      void updateOriginalPhotoPath(string path);

      void updateMaskPhotoPath(string path);

};

#endif // WINDOW_H