#include "LongExposureConfig.h"

LongExposureConfig::LongExposureConfig()
{
    mainLayout = new QVBoxLayout(this);
    
    intervalEdit = new TimeEdit("Interval:");
    lengthEdit = new TimeEdit("Length:");

    mainLayout->addWidget(intervalEdit);
    mainLayout->addWidget(lengthEdit);

    buttonLayout = new QHBoxLayout(this);
    okButton = new QPushButton("OK");
    cancelButton = new QPushButton("Cancel");

    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(okButton);
    mainLayout->addLayout(buttonLayout);

    buttonLayout->setAlignment(Qt::AlignRight);

    this->setLayout(mainLayout);
}