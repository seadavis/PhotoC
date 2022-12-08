#include "fileSelector.h"
#include <iostream>
#include <QFileDialog>
#include <string>
#include <regex>
#include <QString>

using namespace std;

void FileSelector::selectFile()
{
   QFileDialog dialog(this);
   dialog.setFileMode(QFileDialog::AnyFile);
   //dialog.setNameFilter(tr("Images (*.png )"));

   errorLabel->setText("");
   statusImg->setPixmap(QPixmap());

   if(dialog.exec())
   {
        auto selectedFiles = dialog.selectedFiles();
        
        if(selectedFiles.count() > 0)
        {
            auto selectedFile = selectedFiles.constFirst().toStdString();
            regex expression("^.*\\.(png|PNG)$");

            if(regex_match(selectedFile, expression))
            {
                statusImg->setPixmap(QPixmap(":check_mark.png"));
            }
            else
            {
                statusImg->setPixmap(QPixmap(":error_icon.png"));
                errorLabel->setText("File Selected is not a PNG image");
            }
            
            sourceEdit->setText(QString(selectedFile.c_str()));
        }

   }
}

FileSelector::FileSelector()
{
    mainLayout = new QHBoxLayout(this);
    sourceEdit = new QLineEdit;
    sourceEdit->setDisabled(true);

    errorLabel = new QLabel;

    fileTextLayout = new QVBoxLayout;
    fileTextLayout->addWidget(sourceEdit);
    sourceEdit->setAlignment(Qt::AlignTop);

    fileTextLayout->addWidget(errorLabel);
    errorLabel->setAlignment(Qt::AlignTop);
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