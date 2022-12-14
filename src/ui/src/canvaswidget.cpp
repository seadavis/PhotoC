#include <QPixmap>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include "processing.h"
#include "canvaswidget.h"

using namespace cv;
using namespace std;
using namespace processing;

void CanvasWidget::handleButton()
{
 
    Mat img =  camera->snap_picture();
    Mat mask = imread("./src/processing/tests/masks/eagle.png", IMREAD_UNCHANGED);
    Mat source_raw = imread("./src/processing/tests/original_source_images/eagle.png", IMREAD_UNCHANGED);

    Mat tgt;
    cvtColor(img, tgt, CV_BGR2BGRA);

    Mat src;
    cvtColor(source_raw, src, CV_BGR2BGRA);

    Mat comp = composite(mask, src, tgt, 450, 300);

    // the very last step in the process.
    Mat canvas = make_canvas(comp, label->geometry().width() - 50, label->geometry().height() - 50);
    Mat print;

    cvtColor(canvas, print, CV_BGR2RGB);
    image = new QImage(print.data, print.cols, print.rows, QImage::Format_RGB888);
    label->setPixmap(QPixmap::fromImage(*image));
}


CanvasWidget::CanvasWidget(QWidget *parent, ICamera* camera) : QWidget(parent)
{
    verticalLayout = new QVBoxLayout(this);
    this->camera = camera;
    label = new QLabel;
    
    label->setStyleSheet("QLabel { background-color : black; }");

    button = new QPushButton("Snap!");

    verticalLayout->addWidget(label);
    verticalLayout->addWidget(button);

    connect(button, &QPushButton::released, this, &CanvasWidget::handleButton);
}