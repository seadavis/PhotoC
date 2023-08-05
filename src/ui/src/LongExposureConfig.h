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
        void HideTime();
        void ShowTime();

    private slots:
        void handleOKButton();
        void handleCancelButton();
        void handleIntervalIsValid(bool isValid);
        void handleLengthIsValid(bool isValid);

    private:
        TimeEdit* intervalEdit;
        TimeEdit* lengthEdit;
        QVBoxLayout* mainLayout;
        QComboBox* methodSelection;
        QHBoxLayout* buttonLayout;
        QPushButton* okButton;
        QPushButton* cancelButton;
        bool timeIsHidden;
        bool lengthIsValid;
        bool intervalIsValid;

        void setOkButtonState();
};