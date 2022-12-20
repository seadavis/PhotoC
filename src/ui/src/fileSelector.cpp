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
   dialog.setNameFilter(tr("Images (*.png )"));

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
                setFile("");
            }
            
            setFile(selectedFile);
            sourceEdit->setText(QString(selectedFile.c_str()));
            return;
        }

   }
   setFile("");
}

void FileSelector::setFile(string path)
{
    emit fileChanged(path);
}

void FileSelector::setTitle(string title)
{
    this->title->setText(QString(title.c_str()));
}

FileSelector::FileSelector()
{
    grid = new QGridLayout(this);
    sourceEdit = new QLineEdit;
    sourceEdit->setDisabled(true);

    errorLabel = new QLabel;
    title = new QLabel;

    errorLabel->setStyleSheet("QLabel{color: red;}");

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

    grid->addWidget(statusImg, 1, 0);
    grid->addWidget(title, 0, 1);
    grid->addWidget(sourceEdit, 1, 1);
    grid->addWidget(folderImg, 1, 2);
    grid->addWidget(errorLabel, 2, 1);

}