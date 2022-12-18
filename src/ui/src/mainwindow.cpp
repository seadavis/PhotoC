#include "mainwindow.h"
#include <QtWidgets>
#include <QToolBar>

void MainWindow::updateOriginalPhotoPath(string path)
{
  canvas->setOriginalPath(path);
}

void MainWindow::updateMaskPhotoPath(string path)
{
    canvas->setMaskPath(path);
}


MainWindow::MainWindow(ICamera* camera) 
{
 
  this->camera = camera;
  if(!camera->connect())
      return;
  QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
  
  canvas = new CanvasWidget(this, camera);
  setCentralWidget(canvas);

  leftDock = new QDockWidget(tr("Composite Files"), this);
  leftDock->setMinimumWidth(0.4*this->geometry().width());
  leftDock->setAllowedAreas(Qt::LeftDockWidgetArea
                                  | Qt::RightDockWidgetArea);

  compositeSelection = new CompositeSelection();
  addDockWidget(Qt::RightDockWidgetArea, leftDock);
  leftDock->setWidget(compositeSelection);

  connect(compositeSelection, &CompositeSelection::originalPathChanged, this, &MainWindow::updateOriginalPhotoPath);
  connect(compositeSelection, &CompositeSelection::maskPathChanged, this, &MainWindow::updateMaskPhotoPath);
}

