#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QString>
#include "camera.h"
#pragma once

class TimeEdit : public QWidget
{
    Q_OBJECT

    public:
        TimeEdit(QString label);
        void Reset();

        TimeLength GetTimeLength();

    signals:
        void timeEditIsValidChanged(bool isValid);

    private slots:
        void handleTextChanged(const QString &text);

    private:

        QVBoxLayout *rootLayout;
        QHBoxLayout *lineEditLayout;
        QHBoxLayout *errorMessageLayout;
        QLabel *label;
        QLineEdit* edit;
        QLabel* errorImage;
        QLabel* errorMessage;


};