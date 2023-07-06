#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include "canvaswidget.h"
#include "compositeselection.h"
#include "LongExposureDetails.h"

class MainWindow : public QMainWindow
{

   Q_OBJECT

   public:
  
      MainWindow(ICamera* camera);

   private:
      CanvasWidget* canvas;
      QPushButton* button;   
      QDockWidget* leftDock;
      QTabWidget* dockTabs;
      CompositeSelection* compositeSelection;
      LongExposureDetails* longExposureDetails;

      void updateOriginalPhotoPath(string path);
      void updateCameraConnectingStatus(bool isConnecting);
      void updateMaskPhotoPath(string path);

};

#endif // WINDOW_H