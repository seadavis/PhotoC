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

void CanvasWidget::setMaskPath(string path)
{
    maskPath = path;
}

void CanvasWidget::setOriginalPath(string path)
{
   originalPath = path;
}

void CanvasWidget::handleButton()
{ 

    if(maskPath.length() == 0 || originalPath.length() == 0)
        return;

    int canvasHeight = SIZE_FACTOR*this->height();
    int canvasWidth = SIZE_FACTOR*this->width();

    Mat img =  camera->snap_picture();
    canvas->setSize(canvasWidth, canvasHeight);
    canvas->setBackground(img);
    canvas->setComposite(maskPath, originalPath);
    Mat canvasImg = canvas->currentImg();
   
    Mat print;

    cvtColor(canvasImg, print, CV_BGR2RGB);

    imwrite("./test.png", canvasImg);
    imwrite("./test_print.png", print);
    image = new QImage(print.data, print.cols, print.rows, print.step, QImage::Format_RGB888);
    canvasLabel->setPixmap(QPixmap::fromImage(*image));
    canvasLabel->setFixedSize(canvasWidth, canvasHeight);
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