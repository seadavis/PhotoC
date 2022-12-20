#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QImage>
#include <QLineEdit>
#include <QGroupBox>
#include <string>
#include "fileSelector.h"
#pragma once

using namespace std;

class CompositeSelection : public QWidget
{
    Q_OBJECT

    public:
        CompositeSelection();

    protected:
      virtual void resizeEvent(QResizeEvent* resizeEvent) override;

    public slots:
        void setMaskPath(string filePath);
        void setOriginalPath(string filePath);

    signals:
        void maskPathChanged(string filePath);
        void originalPathChanged(string filePath);

    private:
        QVBoxLayout* vBoxLayout;
        QGroupBox* groupBox;
        FileSelector* maskSelector;
        FileSelector* originalSelector;

};