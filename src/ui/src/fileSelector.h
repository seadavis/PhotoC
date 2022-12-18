#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QImage>
#include <QLineEdit>
#include <string>
#pragma once

using namespace std;

class FileSelector : public QWidget
{
    Q_OBJECT

    public:
        FileSelector();

    public slots:
        void setFile(string path);

    signals:
        void fileChanged(string path);

    private slots:
      void selectFile();

    private:
        QHBoxLayout *mainLayout;
        QVBoxLayout *fileTextLayout;
        QLineEdit* sourceEdit;
        QLabel* statusImg;
        QPushButton* folderImg;
        QLabel* errorLabel;
};