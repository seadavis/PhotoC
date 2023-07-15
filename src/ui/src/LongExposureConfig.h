#include <QPushButton>
#include <QDialog>
#include <QComboBox>
#include "TimeEdit.h"
#include "LongExposureStructures.h"
#pragma once

class LongExposureConfig : public QDialog
{
    Q_OBJECT

    public:
        LongExposureConfig();
        void Reset();
        LongExposureDefinition GetLongExposure();

    private slots:
        void handleOKButton();
        void handleCancelButton();

    private:

        TimeEdit* intervalEdit;
        TimeEdit* lengthEdit;
        QVBoxLayout* mainLayout;
        QComboBox* methodSelection;
        QHBoxLayout* buttonLayout;
        QPushButton* okButton;
        QPushButton* cancelButton;
};