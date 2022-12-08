#include "fileSelector.h"

FileSelector::FileSelector()
{
    mainLayout = new QHBoxLayout(this);
    sourceEdit = new QLineEdit;
    sourceEdit->setDisabled(true);

    errorLabel = new QLabel(this);

    fileTextLayout = new QVBoxLayout;
    fileTextLayout->addWidget(sourceEdit);
    sourceEdit->setAlignment(Qt::AlignTop);

    fileTextLayout->addWidget(errorLabel);
    errorLabel->setAlignment(Qt::AlignBottom);
    errorLabel->setStyleSheet("QLabel{color: red;}");
    fileTextLayout->setContentsMargins(0, 0, 0, 5);

    statusImg = new QLabel(this);

    statusImg->setMinimumHeight(25);
    statusImg->setMinimumWidth(25);

    folderImg = new QLabel(this);
    folderImg->setMinimumHeight(25);
    folderImg->setMinimumWidth(25);
    folderImg ->setPixmap(QPixmap(":folder_icon.jpg"));

    mainLayout->addWidget(statusImg);
    mainLayout->addLayout(fileTextLayout);
    mainLayout->addWidget(folderImg);
    statusImg->setAlignment(Qt::AlignTop);
    folderImg->setAlignment(Qt::AlignTop);

}