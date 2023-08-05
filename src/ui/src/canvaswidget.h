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
#include "LongExposureConfig.h"
#include "camera.h"
#include "utilities.h"

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


class StackedImageUpdate : public ICanvasOperator
{

  public:
      StackedImageUpdate(Mat image, shared_ptr<IStackImages> stacker) :
          image(image), stacker(stacker)
      {};

      void Operate(CompositeCanvas& canvas) override;
  
  private:
    Mat image;
    shared_ptr<IStackImages> stacker;

};

class CanvasWidget : public QWidget, public IReceiveImages, public IReceiveErrorMessages, public IRenderImages
{
    Q_OBJECT

    public:
      CanvasWidget(QWidget *parent, ICamera* camera);
      void setMaskPath(string path);
      void setOriginalPath(string path);
      void Receive(Mat m);

      void Receive(string error);

    public slots:
      void showErrorMessage(QString error);
      void RenderImage(Mat &m);
      void RenderStarted();
      void RenderStopped();
      Mat getLastRenderedImage();

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
      void handleLongExposureButton();
      void handleLongExposureAccept();
      
    private:
        shared_ptr<CanvasManager> canvasManager;
        shared_ptr<CompositeCanvas> canvas;
        shared_ptr<QTRenderer> renderer;
        QVBoxLayout* verticalLayout;
        QHBoxLayout* buttonLayout;
        QGridLayout* canvasGrid;
        ImageViewer* canvasViewer;
        LongExposureConfig* longExposureWindow;
        QLabel* backLabel;
        QPushButton* snapButton;  
        QPushButton* connectButton; 
        QPushButton* liveViewButton;
        QPushButton* longExposureButton;
        ICamera *camera;
        int canvasWidth;
        int canvasHeight;
        string maskPath;
        string originalPath;
        bool isInLiveView;
        bool isInLongExposure;
        StackerFactory factory;
        shared_ptr<IStackImages> stacker;
        QMessageBox* longRenderMessageBox;
        atomic<bool> msgBoxDisplayed;
        atomic<bool> isKilled;
        ulong currentRenderNumber;
        mutex renderNumberMutex;
        mutex copyRenderMutex;
        Mat lastRenderedImage;
        int prev_mouse_x = -1;
        int prev_mouse_y = -1;
        
        void displayLiveView(Mat m);
        void sendCompositeUpdate();
        void cameraConnectingStatusChanged(bool isConnecting);
        void renderTimeMonitor();
        std::thread renderTimeMonitorThread;
        void render();
        void showLongExposureConfig(bool isInDefinite);

};