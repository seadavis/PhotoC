#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QImage>
#include <memory>
#include <vector>
#include <QMouseEvent>
#include <QMessageBox>
#include <functional>
#include <atomic>
#include <thread>
#include <mutex>
#include "ImageViewer.h"
#include "canvasmanager.h"
#include "camera.h"

#pragma once

using namespace cv;
using namespace std;
using namespace processing;

class QTTransformImage : public TransformImage
{
  
  public:
    QTTransformImage(Point p, int dx, int dy, QWidget* widget) : TransformImage(p, dx, dy), widget(widget)
    {};

    void OnHit(ObjectType type);

  private:
    QWidget* widget;
};

class CanvasWidget : public QWidget, public IReceiveImages, public IRenderImages
{
    Q_OBJECT

    public:
      CanvasWidget(QWidget *parent, ICamera* camera);
      void setMaskPath(string path);
      void setOriginalPath(string path);
      void Receive(Mat m);
      void RenderImage(Mat &m);
      void RenderStarted();
      void RenderStopped();

      ~CanvasWidget();

    public slots:
      void displayLoadingWindow();
      void hideLoadingWindow();

    protected:
      virtual void resizeEvent(QResizeEvent* resizeEvent) override;

    private slots:
      void handleSnapButton();
      void handleConnectButton();     
      void handleMouseMoveOnImage(int x, int y);
      void handleMousePressOnImage(int x, int y);
      void handleMouseReleaseOnImage(int x, int y);
      void handleLiveViewButton();

    private:
        unique_ptr<CanvasManager> canvasManager;
        unique_ptr<CompositeCanvas> canvas;
        QVBoxLayout* verticalLayout;
        QHBoxLayout* buttonLayout;
        QGridLayout* canvasGrid;
        ImageViewer* canvasViewer;
        QLabel* backLabel;
        QPushButton* snapButton;  
        QPushButton* connectButton; 
        QPushButton* liveViewButton;
        ICamera *camera;
        int canvasWidth;
        int canvasHeight;
        string maskPath;
        string originalPath;
        bool isInLiveView;
        QMessageBox* longRenderMessageBox;
        atomic<bool> msgBoxDisplayed;
        atomic<bool> isKilled;
        ulong currentRenderNumber;
        mutex renderNumberMutex;

        int prev_mouse_x = -1;
        int prev_mouse_y = -1;

        void showErrorMessage(const exception& ex);
        void displayLiveView(Mat m);
        void sendCompositeUpdate();
        void cameraConnectingStatusChanged(bool isConnecting);
        void renderTimeMonitor();
        std::thread renderTimeMonitorThread;
        void render();

};