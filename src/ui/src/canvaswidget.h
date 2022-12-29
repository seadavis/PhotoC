#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QImage>
#include <memory>
#include <vector>
#include <QMouseEvent>
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
      virtual void mousePressEvent(QMouseEvent *mouseEvent) override;
      virtual void mouseMoveEvent(QMouseEvent *mouseEvent) override;
      virtual void mouseReleaseEvent(QMouseEvent *mouseEvent) override;

      tuple<int, int> qtToCanvasCoords(int mouse_x, int mouse_y);

    private slots:
      void handleButton();

    private:
        unique_ptr<CompositeCanvas> canvas;
        QVBoxLayout* verticalLayout;
        QGridLayout* canvasGrid;
        QLabel* canvasLabel;
        QLabel* backLabel;
        QPushButton* button;   
        QImage *image;
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