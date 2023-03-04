#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QImage>
#include <memory>
#include <vector>
#include <QMouseEvent>
#include "ImageViewer.h"
#include "processing.h"
#include "camera.h"

using namespace cv;
using namespace std;
using namespace processing;

class CanvasWidget : public QWidget
{
    public:
      CanvasWidget(QWidget *parent, ICamera* camera);

      void setMaskPath(string path);

      void setOriginalPath(string path);

    protected:
      virtual void resizeEvent(QResizeEvent* resizeEvent) override;

    private slots:
      void handleSnapButton();
      void handleConnectButton();
      
      void handleMouseMoveOnImage(int x, int y);
      void handleMousePressOnImage(int x, int y);
      void handleMouseReleaseOnImage(int x, int y);

    private:
        unique_ptr<CompositeCanvas> canvas;
        QVBoxLayout* verticalLayout;
        QHBoxLayout* buttonLayout;
        QGridLayout* canvasGrid;
        ImageViewer* canvasViewer;
        QLabel* backLabel;
        QPushButton* snapButton;  
        QPushButton* connectButton; 
        ICamera *camera;
        int canvasWidth;
        int canvasHeight;
        string maskPath;
        string originalPath;

        int prev_mouse_x = -1;
        int prev_mouse_y = -1;

        void setCompositesIfAvailable();

        void render();

};