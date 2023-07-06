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
  QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
  
  canvas = new CanvasWidget(this, camera);
  setCentralWidget(canvas);

  leftDock = new QDockWidget(this);
  dockTabs = new QTabWidget;
  leftDock->setMinimumWidth(0.4*this->geometry().width());
  leftDock->setAllowedAreas(Qt::LeftDockWidgetArea
                                  | Qt::RightDockWidgetArea);

  compositeSelection = new CompositeSelection();
  longExposureDetails = new LongExposureDetails();
  addDockWidget(Qt::RightDockWidgetArea, leftDock);
  leftDock->setWidget(dockTabs);
  dockTabs->addTab(compositeSelection, "Composites");
  dockTabs->addTab(longExposureDetails, "Long Exposure Details");

  connect(compositeSelection, &CompositeSelection::originalPathChanged, this, &MainWindow::updateOriginalPhotoPath);
  connect(compositeSelection, &CompositeSelection::maskPathChanged, this, &MainWindow::updateMaskPhotoPath);
}

