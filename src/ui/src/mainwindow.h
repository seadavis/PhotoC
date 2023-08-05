#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QFileDialog>
#include "canvaswidget.h"
#include "compositeselection.h"
#include "LongExposureDetails.h"

class MainWindow : public QMainWindow
{
   Q_OBJECT

   public:
  
      MainWindow(ICamera* camera);

   private slots:
      void save();
      void saveDialogAccepted(const QString& file);

   private:
      CanvasWidget* canvas;
      QPushButton* button;   
      QDockWidget* leftDock;
      QTabWidget* dockTabs;
      CompositeSelection* compositeSelection;
      LongExposureDetails* longExposureDetails;
      QFileDialog* fileDialog;
      Mat matToSave;
      
      void updateOriginalPhotoPath(string path);
      void updateCameraConnectingStatus(bool isConnecting);
      void updateMaskPhotoPath(string path);
};

#endif // WINDOW_H