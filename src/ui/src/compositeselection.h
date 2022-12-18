#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QImage>
#include <QLineEdit>
#include <string>
#include "fileSelector.h"
#pragma once

using namespace std;

class CompositeSelection : public QWidget
{
    Q_OBJECT

    public:
        CompositeSelection();

    public slots:
        void setMaskPath(string filePath);
        void setOriginalPath(string filePath);

    signals:
        void maskPathChanged(string filePath);
        void originalPathChanged(string filePath);

    private:
        QVBoxLayout* layout;

        QVBoxLayout* sourceLayout;
        QVBoxLayout* targetLayout;

        QLabel* sourceLabel;
        QLabel *targetLabel;

        FileSelector* sourceSelector;
        FileSelector* targetSelector;

};