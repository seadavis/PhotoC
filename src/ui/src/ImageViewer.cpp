#include <iostream>
#include "ImageViewer.h"

ImageViewer::ImageViewer()
{
    this->setStyleSheet("QLabel { background-color : black; }");
    this->setMouseTracking(true);
}

void ImageViewer::setImage(Mat m)
{
    Mat print;
    cvtColor(m, print, CV_BGR2RGB);
    auto image = QImage(print.data, print.cols, print.rows, print.step, QImage::Format_RGB888);
    this->setPixmap(QPixmap::fromImage(image));
}

void ImageViewer::mouseMoveEvent(QMouseEvent *mouseEvent)
{
    mouseMove(mouseEvent);
}

void ImageViewer::mouseMove(QMouseEvent* mouseEvent)
{
    emit mouseMoved(mouseEvent->x(), mouseEvent->y());
}

void ImageViewer::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
    mouseRelease(mouseEvent);
}

void ImageViewer::mouseRelease(QMouseEvent* mouseEvent)
{
    emit mouseReleased(mouseEvent->x(), mouseEvent->y());
}

void ImageViewer::mousePressEvent(QMouseEvent *mouseEvent)
{
    mousePress(mouseEvent);
}

void ImageViewer::mousePress(QMouseEvent* mouseEvent)
{
    emit mousePressed(mouseEvent->x(), mouseEvent->y());
}