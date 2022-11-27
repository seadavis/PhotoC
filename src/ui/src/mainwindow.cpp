#include "mainwindow.h"
#include <QtWidgets>
#include <QPushButton>
#include <QImageReader>
#include <QStackedLayout>
#include <QToolBar>
#include <QComboBox>
#include <QDockWidget>

using namespace cv;
using namespace std;


MainWindow::MainWindow(ICamera* camera) 
{
 
  this->camera = camera;
  if(!camera->connect())
      return;
  QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
  
  //QDockWidget* w = new QDockWidget(this);
  canvas = new CanvasWidget(this, camera);
  setCentralWidget(canvas);
}

