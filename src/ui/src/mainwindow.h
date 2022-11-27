#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QMainWindow>
#include "canvaswidget.h"

class MainWindow : public QMainWindow
{

   Q_OBJECT

   public:
  
      MainWindow(ICamera* camera);

   private:
      ICamera* camera;
      CanvasWidget* canvas;
      QPushButton* button;   

};

#endif // WINDOW_H