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
    frame->setMinimumHeight(15);
    frame->setMinimumWidth(15);
    frame->setStyleSheet("QWidget{background-image: url(:/resources/images/check_mark.png} ");
    hLayout->addWidget(frame);
    hLayout->addWidget(sourceEdit);

    sourceLayout->addWidget(sourceLabel);
}