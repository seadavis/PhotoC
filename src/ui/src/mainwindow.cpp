#include "mainwindow.h"
#include <QtWidgets>
#include <QToolBar>
#include "files.h"
#include "utilities.h"

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
  matToSave = canvas->getLastRenderedImage();
  fileDialog->open();
}

void MainWindow::saveDialogAccepted(const QString& fileSelected)
{

  try
  {
    if(fileSelected.length() > 0)
    {
      io::save(matToSave, fileSelected.toStdString());
    }

  }
  catch(const exception &ex)
  {
    showErrorMessage(this, ex);
  }

}

MainWindow::MainWindow(ICamera* camera) 
{ 
  QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

  QAction* saveAction = fileMenu->addAction(tr("&Save"));
  
  canvas = new CanvasWidget(this, camera);
  setCentralWidget(canvas);

  fileDialog = new QFileDialog();
  fileDialog->setAcceptMode(QFileDialog::AcceptSave);
  connect(fileDialog, &QFileDialog::fileSelected, this, &MainWindow::saveDialogAccepted);

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

