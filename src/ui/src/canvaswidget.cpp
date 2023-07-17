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

QTRenderer::QTRenderer(ImageViewer* viewer)
{
    this->viewer = viewer;
}

void QTRenderer::RenderImage(Mat& m)
{
    viewer->setImage(m);
}

void QTTransformImage::OnHit(ObjectType type)
{
     if(type == ObjectType::Image)
    {
        widget->setCursor(QCursor(Qt::CursorShape::SizeAllCursor));
    }
    else if (type == ObjectType::SizeCircle)
    {
        widget->setCursor(QCursor(Qt::CursorShape::SizeFDiagCursor));
    }
    else if(type == ObjectType::None)
    {
        widget->setCursor(QCursor(Qt::CursorShape::ArrowCursor));
    }
}

void CanvasWidget::handleMouseMoveOnImage(int x, int y)
{

    int delta_x = x - prev_mouse_x;
    int delta_y = y - prev_mouse_y;
    canvasManager->QueueOperation(make_shared<QTTransformImage>(Point(x,y), delta_x, delta_y, this));
    prev_mouse_x = x;
    prev_mouse_y = y;
}

void CanvasWidget::handleMouseReleaseOnImage(int x, int y)
{
    setCursor(QCursor(Qt::CursorShape::ArrowCursor));
    canvasManager->QueueOperation(make_shared<ReleaseImage>());
}

void CanvasWidget::handleMousePressOnImage(int x, int y)
{
    canvasManager->QueueOperation(make_shared<TapImage>(Point(x, y)));
    prev_mouse_x = x;
    prev_mouse_y = y;
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
    sendCompositeUpdate();
}

void CanvasWidget::setOriginalPath(string path)
{
    originalPath = path;
    sendCompositeUpdate();
}

void CanvasWidget::sendCompositeUpdate()
{
    auto update = CompositeImageUpdate(originalPath, maskPath);
    canvasManager->QueueOperation(make_shared<CompositeImageUpdate>(originalPath, maskPath));
}

void CanvasWidget::handleSnapButton()
{  
    try
    {
        Mat img =  camera->snap_picture();
        canvasManager->QueueOperation(make_shared<BackgroundImageUpdate>(img));
    }
    catch(const exception &ex)
    {
        showErrorMessage(ex);
    }
}

void CanvasWidget::handleConnectButton()
{
    try
    {   
        cameraConnectingStatusChanged(true);
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        camera->connect();
        camera->SetReceiver(this);
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
    liveViewButton->setEnabled(!isConnecting);
    longExposureButton->setEnabled(!isConnecting);
}

void CanvasWidget::showErrorMessage(const exception& ex)
{
    auto msg = new QErrorMessage(this);
    auto what = ex.what();
    msg->showMessage(QString(what));
}

void CanvasWidget::handleLongExposureButton()
{
    if(isInLongExposure)
    {
        this->camera->StopLongExposure();
        isInLongExposure = false;
    }
    else
    {
        longExposureWindow->Reset();
        longExposureWindow->show();
    }
}

void CanvasWidget::handleLongExposureAccept()
{
    longExposureButton->setText("Stop long Exposure");
    auto longExposureDef = longExposureWindow->GetLongExposure();
    stacker = factory.CreateStacker(longExposureDef);
    this->camera->StartLongExposure(longExposureDef.shots);
    isInLongExposure = true;
}

void CanvasWidget::handleLongExposureReject()
{
    int test = 32;
}

void CanvasWidget::handleLiveViewButton()
{
    try
    {
        if(isInLiveView)
        {
            camera->StopLiveView();
            liveViewButton->setText("Live View");
            isInLiveView = false;
        }
        else
        {
            camera->StartLiveView();
            liveViewButton->setText("Stop Live View");
            isInLiveView = true;
        }
       
    }
    catch(const exception &ex)
    {
       showErrorMessage(ex);
    }
}

void CanvasWidget::Receive(Mat img)
{
    if(isInLongExposure)
    {
        stacker->AddToStack(img);
        Mat blend = stacker->GetCurrentBlend();
        canvasManager->QueueOperation(make_shared<BackgroundImageUpdate>(blend));
    }
    else
    {
        canvasManager->QueueOperation(make_shared<BackgroundImageUpdate>(img));
    }
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
    liveViewButton = new QPushButton("Live View");
    longExposureButton = new QPushButton("Long Exposure");

    snapButton->setEnabled(false);
    liveViewButton->setEnabled(false);
    longExposureButton->setEnabled(false);

    buttonLayout->addWidget(snapButton);
    buttonLayout->addWidget(connectButton);
    buttonLayout->addWidget(liveViewButton);
    buttonLayout->addWidget(longExposureButton);

    canvasViewer->setFixedSize(0, 0);
    canvasGrid->addWidget(backLabel, 0, 0);
    canvasGrid->addWidget(canvasViewer, 0, 0, Qt::AlignCenter);
    verticalLayout->addLayout(canvasGrid);
    verticalLayout->addLayout(buttonLayout);
    buttonLayout->setAlignment(Qt::AlignCenter);
    canvas = unique_ptr<CompositeCanvas>(new CompositeCanvas());
    renderer = unique_ptr<QTRenderer>(new QTRenderer(canvasViewer));
    canvasManager = unique_ptr<CanvasManager>(new CanvasManager(canvas.get(), renderer.get()));

    isInLiveView = false;

    longExposureWindow = new LongExposureConfig;

    connect(longExposureWindow, &LongExposureConfig::accepted, this, &CanvasWidget::handleLongExposureAccept);
    connect(longExposureWindow, &LongExposureConfig::rejected, this, &CanvasWidget::handleLongExposureReject);
    connect(liveViewButton, &QPushButton::released, this, &CanvasWidget::handleLiveViewButton);
    connect(snapButton, &QPushButton::released, this, &CanvasWidget::handleSnapButton);
    connect(longExposureButton, &QPushButton::released, this, &CanvasWidget::handleLongExposureButton);
    connect(connectButton, &QPushButton::released, this, &CanvasWidget::handleConnectButton);
    connect(canvasViewer, &ImageViewer::mouseMoved, this, &CanvasWidget::handleMouseMoveOnImage);
    connect(canvasViewer, &ImageViewer::mousePressed, this, &CanvasWidget::handleMousePressOnImage);
    connect(canvasViewer, &ImageViewer::mouseReleased, this, &CanvasWidget::handleMouseReleaseOnImage);
}