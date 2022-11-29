#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QImage>

#include "camera.h"

class CanvasWidget : public QWidget
{
    public:
       CanvasWidget(QWidget *parent, ICamera* camera);

    private slots:
      void handleButton();

    private:
        QVBoxLayout* verticalLayout;
        QLabel* label;
        QPushButton* button;   
        QImage *image;
        ICamera *camera;

};