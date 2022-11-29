#include "compositeselection.h"

CompositeSelection::CompositeSelection()
{
    layout = new QVBoxLayout(this);
    sourceLayout = new QVBoxLayout;

    sourceEdit = new QLineEdit;
    sourceEdit->setDisabled(true);

    sourceLabel = new QLabel;
    sourceLabel->setText("Error");

    layout->addLayout(sourceLayout);

    sourceLayout->addWidget(sourceEdit);
    sourceLayout->addWidget(sourceLabel);

    frame = new QWidget(this);
    frame->setMinimumHeight(10);
    frame->setMinimumWidth(10);
    frame->setStyleSheet("QWidget{background-image: url(:/resources/images/check_mark.jpg)} ");
    //frame->setStyleSheet("background-image: url(:check_mark.jpg)");
    sourceLayout->addWidget(frame);
}