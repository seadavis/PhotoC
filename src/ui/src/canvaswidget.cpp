#include <QPixmap>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <QCursor>
#include <QErrorMessage>
#include <QMessageBox>
#include <QCoreApplication>
#include "canvaswidget.h"

constexpr double SIZE_FACTOR = 0.65;

void CanvasWidget::setCompositesIfAvailable()
{
    if(maskPath.length() != 0 && originalPath.length() != 0)
    {
        canvas->setComposite(maskPath, originalPath);
        render();
    }
}

void CanvasWidget::handleMouseMoveOnImage(int x, int y)
{
    auto hitType = canvas->hit(Point(x, y));

    if(hitType == ObjectType::Image)
    {
        setCursor(QCursor(Qt::CursorShape::SizeAllCursor));
    }
    else if (hitType == ObjectType::SizeCircle)
    {
        setCursor(QCursor(Qt::CursorShape::SizeFDiagCursor));
    }
    else if(hitType == ObjectType::None)
    {
        setCursor(QCursor(Qt::CursorShape::ArrowCursor));
    }

    int delta_x = x - prev_mouse_x;
    int delta_y = y - prev_mouse_y;

    prev_mouse_x = x;
    prev_mouse_y = y;

    canvas->cursorMoved(delta_x, delta_y);
    render();
}

void CanvasWidget::handleMouseReleaseOnImage(int x, int y)
{
    setCursor(QCursor(Qt::CursorShape::ArrowCursor));
    canvas->releaseObject();
    render();
}

void CanvasWidget::handleMousePressOnImage(int x, int y)
{
    canvas->tap(Point(x, y));
    render();

    prev_mouse_x = x;
    prev_mouse_y = y;
}

void CanvasWidget::render()
{
    canvasViewer->setImage(canvas->currentImg());
}

void CanvasWidget::resizeEvent(QResizeEvent* event) 
{
    canvasHeight = SIZE_FACTOR*this->height();
    canvasWidth = SIZE_FACTOR*this->width();
    canvasViewer->setFixedSize(canvasWidth, canvasHeight);
    canvas->setSize(canvasWidth, canvasHeight);
}

void CanvasWidget::setMaskPath(string path)
{
    maskPath = path;
    setCompositesIfAvailable();
}

void CanvasWidget::setOriginalPath(string path)
{
    originalPath = path;
    setCompositesIfAvailable();
}

void CanvasWidget::handleSnapButton()
{  
    try
    {
        Mat img =  camera->snap_picture();
        canvas->setBackground(img);
        render();
    }
    catch(const exception &ex)
    {
        auto msg = new QErrorMessage(this);
        auto what = ex.what();
        msg->showMessage(QString(what));
    }
}

void CanvasWidget::handleConnectButton()
{
    try
    {   
        cameraConnectingStatusChanged(true);
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        camera->connect();
        auto msg = new QMessageBox(this);
        msg->setWindowTitle("Success!");
        msg->setText("Successfully connected camera");
        msg->show();
        cameraConnectingStatusChanged(false);
        snapButton->setEnabled(true);
    }
    catch(const exception &ex)
    {
        cameraConnectingStatusChanged(false);
        auto msg = new QErrorMessage(this);	
        auto what = ex.what();
        msg->showMessage(QString(what));
        snapButton->setEnabled(false);		
    }
}

void CanvasWidget::cameraConnectingStatusChanged(bool isConnecting)
{   
    connectButton->setEnabled(!isConnecting);
}

CanvasWidget::CanvasWidget(QWidget *parent, ICamera* camera) : QWidget(parent)
{
    verticalLayout = new QVBoxLayout(this);
    buttonLayout = new QHBoxLayout(this);
    this->camera = camera;
    canvasGrid = new QGridLayout(parent);
    backLabel = new QLabel;
    backLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    backLabel->setStyleSheet("QLabel { background-color : grey; }");
    backLabel->setSizePolicy(QSizePolicy::MinimumExpanding,
                     QSizePolicy::MinimumExpanding);

    canvasViewer = new ImageViewer;

    snapButton = new QPushButton("Snap!");
    connectButton = new QPushButton("Connect");
    buttonLayout->addWidget(snapButton);
    buttonLayout->addWidget(connectButton);

    canvasViewer->setFixedSize(0, 0);
    canvasGrid->addWidget(backLabel, 0, 0);
    canvasGrid->addWidget(canvasViewer, 0, 0, Qt::AlignCenter);
    verticalLayout->addLayout(canvasGrid);
    verticalLayout->addLayout(buttonLayout);
    buttonLayout->setAlignment(Qt::AlignCenter);
    canvas = unique_ptr<CompositeCanvas>(new CompositeCanvas());
    snapButton->setEnabled(false);
    connect(snapButton, &QPushButton::released, this, &CanvasWidget::handleSnapButton);
    connect(connectButton, &QPushButton::released, this, &CanvasWidget::handleConnectButton);
    connect(canvasViewer, &ImageViewer::mouseMoved, this, &CanvasWidget::handleMouseMoveOnImage);
    connect(canvasViewer, &ImageViewer::mousePressed, this, &CanvasWidget::handleMousePressOnImage);
    connect(canvasViewer, &ImageViewer::mouseReleased, this, &CanvasWidget::handleMouseReleaseOnImage);
}