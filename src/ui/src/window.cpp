#include "window.h"
#include <QPushButton>
#include <QImageReader>
#include <QStackedLayout>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "processing.h"

using namespace cv;
using namespace std;
using namespace processing;

void Window::handleButton(){
 

  Mat img =  camera->snap_picture();
  Mat out;
  cv::resize(img, out, cv::Size(900, 600));

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
  pixmap = new QPixmap();
  image = new QImage(print.data, print.cols, print.rows, QImage::Format_RGB888);
  label->setPixmap(QPixmap::fromImage(*image));
}

void Window::set_camera(ICamera* camera)
{
  this->camera = camera;
  cout << "Connecting Camera!\n";
    if(!camera->connect())
      return;
}

Window::Window(QWidget *parent) :
 QWidget(parent)
 {
 
    // Set size of the window
    setFixedSize(1500, 1200);

    QVBoxLayout* layout = new QVBoxLayout(this);

   
    label = new QLabel;
    label->setGeometry(10, 10, 901, 676);
    layout->addWidget(label);

    button = new QPushButton("Snap!");
    button->setGeometry(15, 15, 100, 50);
    layout->addWidget(button);

    connect(button, &QPushButton::released, this, &Window::handleButton);
}

