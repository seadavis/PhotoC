#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QImage>
#include <memory>
#include <vector>
#include <QMouseEvent>
#include <functional>
#include "ImageViewer.h"
#include "canvasmanager.h"
#include "camera.h"

using namespace cv;
using namespace std;
using namespace processing;

class QTRenderer : public IRenderImages
{
    public:
      QTRenderer(ImageViewer* viewer);
      void RenderImage(Mat& m) override;

    private:
      ImageViewer* viewer;
};

class QTTransformImage : public TransformImage
{
  
  public:
    QTTransformImage(Point p, int dx, int dy, QWidget* widget) : TransformImage(p, dx, dy), widget(widget)
    {};

    void OnHit(ObjectType type);

  private:
    QWidget* widget;
};

class CanvasWidget : public QWidget, public IReceiveImages
{
    public:
      CanvasWidget(QWidget *parent, ICamera* camera);
      void setMaskPath(string path);
      void setOriginalPath(string path);
      void Receive(Mat m);

      ~CanvasWidget();

    protected:
      virtual void resizeEvent(QResizeEvent* resizeEvent) override;

    public slots:
        

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
        unique_ptr<QTRenderer> renderer;
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

        int prev_mouse_x = -1;
        int prev_mouse_y = -1;

        void showErrorMessage(const exception& ex);
        void displayLiveView(Mat m);
        void sendCompositeUpdate();
        void cameraConnectingStatusChanged(bool isConnecting);
        void render();

};