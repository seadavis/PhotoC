#include "compositeselection.h"

CompositeSelection::CompositeSelection()
{
    layout = new QVBoxLayout(this);
    sourceLayout = new QVBoxLayout;

    hLayout = new QHBoxLayout;

  
    sourceLabel = new QLabel;
    sourceLabel->setText("Error");

    layout->addLayout(sourceLayout);

    sourceLayout->addLayout(hLayout);

    sourceEdit = new QLineEdit;
    sourceEdit->setDisabled(true);


    frame = new QWidget(this);
    frame->setMinimumHeight(10);
    frame->setMinimumWidth(10);
    frame->setStyleSheet("QWidget{background-image: url(:/resources/images/check_mark.jpg)} ");
    hLayout->addWidget(frame);
    hLayout->addWidget(sourceEdit);

    sourceLayout->addWidget(sourceLabel);
}