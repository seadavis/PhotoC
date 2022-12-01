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

  

    frame = new QLabel(this);
    frame->setMinimumHeight(25);
    frame->setMinimumWidth(25);
    frame->setPixmap(QPixmap(":/resources/images/check_mark.png"));
    hLayout->addWidget(frame);
    hLayout->addWidget(sourceEdit);

    sourceLayout->addWidget(sourceLabel);
}