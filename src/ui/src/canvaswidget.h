#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QImage>
#include <memory>
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

        string maskPath;
        string originalPath;

};