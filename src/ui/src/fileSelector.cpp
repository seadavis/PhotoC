#include "fileSelector.h"
#include <iostream>

using namespace std;

void FileSelector::selectFile()
{
    cout << "Hello From File!";
}

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

        

    QPixmap pixmap(":folder_icon.jpg");
    QIcon buttonIcon(pixmap);
    folderImg = new QPushButton;
    folderImg->setMinimumHeight(25);
    folderImg->setMinimumWidth(25);
    folderImg ->setIcon(buttonIcon);

    
    connect(folderImg, &QPushButton::released, this, &FileSelector::selectFile);

    mainLayout->addWidget(statusImg);
    mainLayout->addLayout(fileTextLayout);
    mainLayout->addWidget(folderImg, 0, Qt::AlignTop);
    statusImg->setAlignment(Qt::AlignTop);

}