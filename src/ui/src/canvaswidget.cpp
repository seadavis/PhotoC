#include <QPixmap>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>

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

void CanvasWidget::render()
{
    Mat canvasImg = canvas->currentImg();
    Mat print;
    cvtColor(canvasImg, print, CV_BGR2RGB);
    image = new QImage(print.data, print.cols, print.rows, print.step, QImage::Format_RGB888);
    canvasLabel->setPixmap(QPixmap::fromImage(*image));
}

void CanvasWidget::resizeEvent(QResizeEvent* event) 
{
    canvasHeight = SIZE_FACTOR*this->height();
    canvasWidth = SIZE_FACTOR*this->width();
    canvasLabel->setFixedSize(canvasWidth, canvasHeight);
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

void CanvasWidget::handleButton()
{ 
    Mat img =  camera->snap_picture();
    canvas->setBackground(img);
    render();
}

CanvasWidget::CanvasWidget(QWidget *parent, ICamera* camera) : QWidget(parent)
{
    verticalLayout = new QVBoxLayout(this);
    this->camera = camera;
    canvasGrid = new QGridLayout(parent);
    backLabel = new QLabel;
    backLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    backLabel->setStyleSheet("QLabel { background-color : grey; }");
    backLabel->setSizePolicy(QSizePolicy::MinimumExpanding,
                     QSizePolicy::MinimumExpanding);

    canvasLabel = new QLabel;
    canvasLabel->setStyleSheet("QLabel { background-color : black; }");
   

    button = new QPushButton("Snap!");
    canvasLabel->setFixedSize(0, 0);
    canvasGrid->addWidget(backLabel, 0, 0);
    canvasGrid->addWidget(canvasLabel, 0, 0, Qt::AlignCenter);
    verticalLayout->addLayout(canvasGrid);
    verticalLayout->addWidget(button);
    canvas = unique_ptr<CompositeCanvas>(new CompositeCanvas());

    connect(button, &QPushButton::released, this, &CanvasWidget::handleButton);
}