#include "window.h"
#include <QPushButton>
#include <QImageReader>


Window::Window(QWidget *parent) :
 QWidget(parent)
 {
    // Set size of the window
    setFixedSize(600, 400);

    // Create and position the button
    //m_button = new QPushButton("Hello World", this);
    //m_button->setGeometry(10, 10, 80, 30);

    QImageReader reader;
    image = new QImage("sunflower.jpg");
    label = new QLabel(this);
    label->setGeometry(10, 10, 480, 320);
    label->setPixmap(QPixmap::fromImage(*image));
    
}

