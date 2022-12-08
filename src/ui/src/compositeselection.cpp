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

    errorLabel = new QLabel(this);
    errorLabel->setText("Some Error");

    fileTextLayout = new QVBoxLayout;
    fileTextLayout->addWidget(sourceEdit);
    sourceEdit->setAlignment(Qt::AlignTop);

    fileTextLayout->addWidget(errorLabel);
    errorLabel->setAlignment(Qt::AlignBottom);
    fileTextLayout->setContentsMargins(0, 0, 0, 5);

    frame = new QLabel(this);
    frame->setMinimumHeight(25);
    frame->setMinimumWidth(25);
    frame->setPixmap(QPixmap(":check_mark.png"));

    folderImg = new QLabel(this);
    folderImg->setMinimumHeight(25);
    folderImg->setMinimumWidth(25);
    folderImg ->setPixmap(QPixmap(":folder_icon.jpg"));

    hLayout->addWidget(frame);
    hLayout->addLayout(fileTextLayout);
    hLayout->addWidget(folderImg);
    frame->setAlignment(Qt::AlignTop);
    folderImg->setAlignment(Qt::AlignTop);

    hLayout->setAlignment(Qt::AlignTop);

    sourceLayout->addWidget(sourceLabel);
}