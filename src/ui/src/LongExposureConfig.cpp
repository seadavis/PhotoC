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

    connect(okButton, &QPushButton::released, this, &LongExposureConfig::handleOKButton);
    connect(cancelButton, &QPushButton::released, this, &LongExposureConfig::handleCancelButton);
}

void LongExposureConfig::Reset()
{
    intervalEdit->Reset();
    lengthEdit->Reset();
}

void LongExposureConfig::handleOKButton()
{
    accept();
}

void LongExposureConfig::handleCancelButton()
{
    reject();
}

LongExposureDefinition LongExposureConfig::GetLongExposure()
{

    auto shots = LongExposureShots(intervalEdit->GetTimeLength(), lengthEdit->GetTimeLength());
    auto method = (LongExposureBlendingMethod)methodSelection->currentData().toInt();
    return LongExposureDefinition(shots, method);

}