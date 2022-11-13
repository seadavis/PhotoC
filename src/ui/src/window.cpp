#include "window.h"
#include <QPushButton>
#include <QImageReader>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "camera.h"

using namespace cv;
using namespace std;

void Window::handleButton(){
 
  auto camera = RemoteCamera();
  cout << "Connecting Camera!\n";
  if(!camera.connect())
    return;

  Mat img =  camera.snap_picture();
	Mat out;
	cv::resize(img, out, cv::Size(480, 320));


  cout << "Image Total:  " << img.total() << " Element Size: " << img.elemSize() << "\n";
  cvtColor(out,out,CV_BGR2RGB);
  pixmap = new QPixmap();
  image = new QImage(out.data, out.cols, out.rows, QImage::Format_RGB888);
  label->setPixmap(QPixmap::fromImage(*image));
}

Window::Window(QWidget *parent) :
 QWidget(parent)
 {
    // Set size of the window
    setFixedSize(600, 400);

    label = new QLabel(this);
    label->setGeometry(10, 10, 480, 320);

    button = new QPushButton("Snap!", this);
    button->setGeometry(15, 15, 100, 50);

    connect(button, &QPushButton::released, this, &Window::handleButton);
}

