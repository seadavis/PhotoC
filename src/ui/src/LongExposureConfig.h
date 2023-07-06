#include <QPushButton>
#include <QDialog>
#include "TimeEdit.h"
#pragma once

class LongExposureConfig : public QDialog
{
    Q_OBJECT

    public:
        LongExposureConfig();
        void Reset();
        LongExposure GetLongExposure();

    private slots:
        void handleOKButton();
        void handleCancelButton();

    private:

        TimeEdit* intervalEdit;
        TimeEdit* lengthEdit;
        QVBoxLayout* mainLayout;

        QHBoxLayout* buttonLayout;
        QPushButton* okButton;
        QPushButton* cancelButton;
};