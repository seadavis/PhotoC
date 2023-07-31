#include "LongExposureConfig.h"

LongExposureConfig::LongExposureConfig()
{
    mainLayout = new QVBoxLayout;
    
    intervalEdit = new TimeEdit("Interval:");
    lengthEdit = new TimeEdit("Length:");

    mainLayout->addWidget(intervalEdit);
    mainLayout->addWidget(lengthEdit);

    methodSelection = new QComboBox;
    methodSelection->addItem("Average", (int)LongExposureBlendingMethod::Average);
    methodSelection->addItem("Brighten", (int)LongExposureBlendingMethod::Brighten);

    mainLayout->addWidget(methodSelection);
    buttonLayout = new QHBoxLayout;
    okButton = new QPushButton("OK");
    cancelButton = new QPushButton("Cancel");

    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(okButton);
    mainLayout->addLayout(buttonLayout);
    buttonLayout->setAlignment(Qt::AlignRight);
    this->setModal(true);
    this->setLayout(mainLayout);
    this->okButton->setEnabled(false);

    connect(okButton, &QPushButton::released, this, &LongExposureConfig::handleOKButton);
    connect(cancelButton, &QPushButton::released, this, &LongExposureConfig::handleCancelButton);
    connect(intervalEdit, &TimeEdit::timeEditIsValidChanged, this, &LongExposureConfig::handleIntervalIsValid);
    connect(lengthEdit, &TimeEdit::timeEditIsValidChanged, this, &LongExposureConfig::handleLengthIsValid);
}

void LongExposureConfig::Reset()
{
    intervalEdit->Reset();
    lengthEdit->Reset();
    ShowTime();
}

void LongExposureConfig::HideTime()
{
    intervalEdit->setHidden(true);
    lengthEdit->setHidden(true);
    timeIsHidden = true;
}

void LongExposureConfig::ShowTime()
{
    intervalEdit->setHidden(false);
    lengthEdit->setHidden(false);
    timeIsHidden = false;
}

void LongExposureConfig::handleOKButton()
{
    accept();
}

void LongExposureConfig::handleCancelButton()
{
    reject();
}

void LongExposureConfig::handleIntervalIsValid(bool isValid)
{
    intervalIsValid = isValid;
    setOkButtonState();
}

void LongExposureConfig::handleLengthIsValid(bool isValid)
{
    lengthIsValid = isValid;
    setOkButtonState();
}

void LongExposureConfig::setOkButtonState()
{
    this->okButton->setEnabled(lengthIsValid && intervalIsValid);
}

LongExposureDefinition LongExposureConfig::GetLongExposure()
{
    LongExposureShots shots;
    if(!timeIsHidden)
        shots = LongExposureShots(intervalEdit->GetTimeLength(), lengthEdit->GetTimeLength());
    
    auto method = (LongExposureBlendingMethod)methodSelection->currentData().toInt();
    return LongExposureDefinition(shots, method);

}