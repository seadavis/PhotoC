#include <QWidget>
#include <QLabel>
#include <QImage>
#include <QMouseEvent>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#pragma once

using namespace std;
using namespace cv;

class ImageViewer : public QLabel
{
    Q_OBJECT

    public:
        ImageViewer();
        void setImage(Mat m);

    protected:
        virtual void mouseMoveEvent(QMouseEvent *mouseEvent) override;
        virtual void mousePressEvent(QMouseEvent *mouseEvent) override;
        virtual void mouseReleaseEvent(QMouseEvent *mouseEvent) override;

    signals:
        void mouseMoved(int x, int y);
        void mousePressed(int x, int y);
        void mouseReleased(int x, int y);

    public slots:
        void mouseMove(QMouseEvent *mouseEvent);
        void mousePress(QMouseEvent *mouseEvent);
        void mouseRelease(QMouseEvent *mouseEvent);

};