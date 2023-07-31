#include "TimeEdit.h"


TimeEdit::TimeEdit(QString label)
{

    rootLayout = new QVBoxLayout;

    lineEditLayout = new QHBoxLayout;
    this->label = new QLabel(label);
    edit = new QLineEdit("hh:mm:ss");
    errorMessageLayout = new QHBoxLayout;


    lineEditLayout->addWidget(this->label);
    lineEditLayout->addWidget(edit);
   

    QPixmap pixmap(":error_icon_small.jpg");
    errorImage = new QLabel();
    errorImage->setPixmap(pixmap);
    errorImage->setHidden(true);
    errorMessage = new QLabel();
    errorMessage->setText("Time must be in the format of: hh:mm:ss");
    errorMessage->setHidden(true);

    auto errorMessageSizePolicy = errorMessage->sizePolicy();
    errorMessageSizePolicy.setRetainSizeWhenHidden(true);
    errorMessage->setSizePolicy(errorMessageSizePolicy);

    auto errorImageSizePolicy = errorImage->sizePolicy();
    errorImageSizePolicy.setRetainSizeWhenHidden(true);
    errorImage->setSizePolicy(errorMessageSizePolicy);

    errorMessageLayout->addWidget(errorImage);
    errorMessageLayout->addWidget(errorMessage);

    rootLayout->addLayout(lineEditLayout);
    rootLayout->addLayout(errorMessageLayout);

    this->setLayout(rootLayout);
    this->setWindowTitle("Long Exposure Settings");
    connect(edit, &QLineEdit::textChanged, this, &TimeEdit::handleTextChanged);
}

void TimeEdit::Reset()
{
    edit->setText("hours:minutes:seconds");
}

void TimeEdit::handleTextChanged(const QString& text)
{
    auto time = ParseTimeLength(text.toStdString());
    if(!time.has_value())
    {
        errorImage->setHidden(false);
        errorMessage->setHidden(false);
        emit timeEditIsValidChanged(false);
    }
    else
    {
        errorImage->setHidden(true);
        errorMessage->setHidden(true);
        emit timeEditIsValidChanged(true);
    }
}   

TimeLength TimeEdit::GetTimeLength()
{
    return ParseTimeLength(edit->text().toStdString()).value();
}