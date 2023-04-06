#include "mainwindow.h"
#include <QtWidgets>
#include <QToolBar>
#include <QFileDialog>
#include "files.h"

void MainWindow::updateOriginalPhotoPath(string path)
{
  canvas->setOriginalPath(path);
  
}

void MainWindow::updateMaskPhotoPath(string path)
{
    canvas->setMaskPath(path);
}

void MainWindow::save()
{
    QFileDialog f = QFileDialog();
    f.setAcceptMode(QFileDialog::AcceptSave);
    f.exec();

    QString fileSelected;
    f.fileSelected(fileSelected);

    auto s = fileSelected.toStdString();
    int test = 5;

    //Mat m;
    //auto result = io::save(m, "", true);
}

MainWindow::MainWindow(ICamera* camera) 
{ 
  QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

  QAction* saveAction = fileMenu->addAction(tr("&Save"));
  
  canvas = new CanvasWidget(this, camera);
  setCentralWidget(canvas);

  leftDock = new QDockWidget(tr("Composite Files"), this);
  leftDock->setMinimumWidth(0.4*this->geometry().width());
  leftDock->setAllowedAreas(Qt::LeftDockWidgetArea
                                  | Qt::RightDockWidgetArea);

  compositeSelection = new CompositeSelection();
  addDockWidget(Qt::RightDockWidgetArea, leftDock);
  leftDock->setWidget(compositeSelection);

  connect(saveAction, &QAction::triggered, this, &MainWindow::save);
  connect(compositeSelection, &CompositeSelection::originalPathChanged, this, &MainWindow::updateOriginalPhotoPath);
  connect(compositeSelection, &CompositeSelection::maskPathChanged, this, &MainWindow::updateMaskPhotoPath);
}

