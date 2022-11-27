#include "mainwindow.h"
#include <QtWidgets>
#include <QToolBar>


MainWindow::MainWindow(ICamera* camera) 
{
 
  this->camera = camera;
  if(!camera->connect())
      return;
  QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
  
  canvas = new CanvasWidget(this, camera);
  setCentralWidget(canvas);
}

