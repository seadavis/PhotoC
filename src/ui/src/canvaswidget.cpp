#include <QPixmap>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "processing.h"
#include "canvaswidget.h"

using namespace cv;
using namespace std;
using namespace processing;

void CanvasWidget::handleButton()
{
 
    Mat img =  camera->snap_picture();
    Mat out;

    double canvas_width = label->geometry().width();
    double image_width = img.size().width;
    int widthFactor = canvas_width/image_width;


    double canvas_height = label->geometry().height();
    double image_height = img.size().height;
    int heightFactor = canvas_height/image_height;

    cv::resize(img, out, Size(), widthFactor, heightFactor, CV_INTER_LINEAR);

    Mat mask = imread("./src/processing/tests/masks/eagle.png", IMREAD_UNCHANGED);
    Mat source_raw = imread("./src/processing/tests/original_source_images/eagle.png", IMREAD_UNCHANGED);

    Mat tgt;
    cvtColor(out, tgt, CV_BGR2BGRA);

    Mat src;
    cvtColor(source_raw, src, CV_BGR2BGRA);

    Mat comp = composite(mask, src, tgt, 450, 300);
    Mat print;
    cout << "Image Total:  " << img.total() << " Element Size: " << img.elemSize() << "\n";
    cvtColor(comp,print,CV_BGRA2RGB);
    image = new QImage(print.data, print.cols, print.rows, QImage::Format_RGB888);
    label->setPixmap(QPixmap::fromImage(*image));
}


CanvasWidget::CanvasWidget(QWidget *parent, ICamera* camera) : QWidget(parent)
{
    verticalLayout = new QVBoxLayout(this);
    this->camera = camera;
    label = new QLabel;
    
    label->setStyleSheet("QLabel { background-color : black; }");

    auto blackPixMap = QPixmap();
    blackPixMap.fill(Qt::black);
    label->setPixmap(blackPixMap);

    button = new QPushButton("Snap!");

    verticalLayout->addWidget(label);
    verticalLayout->addWidget(button);

    connect(button, &QPushButton::released, this, &CanvasWidget::handleButton);
}