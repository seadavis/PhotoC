#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "canvaswidget.h"
#include "compositeselection.h"

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
      CompositeSelection* compositeSelection;
      QFileDialog* fileDialog;
      Mat matToSave;

      void updateOriginalPhotoPath(string path);
      void updateCameraConnectingStatus(bool isConnecting);
      void updateMaskPhotoPath(string path);
};

#endif // WINDOW_H