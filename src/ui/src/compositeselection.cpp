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

    folderImg = new QLabel(this);
    folderImg->setMinimumHeight(25);
    folderImg->setMinimumWidth(25);
    folderImg ->setPixmap(QPixmap(":/resources/images/folder_icon.jpg"));

    hLayout->addWidget(frame);
    hLayout->addWidget(sourceEdit);
    hLayout->addWidget(folderImg);

    sourceLayout->addWidget(sourceLabel);
}