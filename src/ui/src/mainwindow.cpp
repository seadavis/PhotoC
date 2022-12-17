#include "mainwindow.h"
#include <QtWidgets>
#include <QToolBar>


MainWindow::MainWindow(ICamera* camera) 
{
 
  this->camera = camera;
  if(!camera->connect())
      return;
  QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
  
  compositePaths = shared_ptr<CompositePaths>(new CompositePaths);
  canvas = new CanvasWidget(this, camera, compositePaths);
  setCentralWidget(canvas);

  leftDock = new QDockWidget(tr("Composite Files"), this);
  leftDock->setMinimumWidth(0.4*this->geometry().width());
  leftDock->setAllowedAreas(Qt::LeftDockWidgetArea
                                  | Qt::RightDockWidgetArea);

 
  compositeSelection = new CompositeSelection(compositePaths);
  addDockWidget(Qt::RightDockWidgetArea, leftDock);
  leftDock->setWidget(compositeSelection);

}

