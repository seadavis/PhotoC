#include "window.h"
#include <QPushButton>
#include <QImageReader>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace cv;
using namespace std;

string type2str(int type) {
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}

void Window::handleButton(){
  cout << "Button Clicked \n";
}

Window::Window(QWidget *parent) :
 QWidget(parent)
 {
    // Set size of the window
    setFixedSize(600, 400);
    
    Mat mat = cv::imread("sunflower.jpg");
    cout << "Image Total:  " << mat.total() << " Element Size: " << mat.elemSize() << "\n";
    cout << "Format: " << type2str(mat.type()) << "\n";
    cvtColor(mat,mat,CV_BGR2RGB);

    label = new QLabel(this);
    label->setGeometry(10, 10, 480, 320);

    button = new QPushButton("New Button", this);
    button->setGeometry(15, 15, 100, 50);

    pixmap = new QPixmap();
    image = new QImage(mat.data, mat.cols, mat.rows, QImage::Format_RGB888);
    label->setPixmap(QPixmap::fromImage(*image));

    connect(button, &QPushButton::released, this, &Window::handleButton);
    
}

