#include "LongExposureDetails.h"

LongExposureDetails::LongExposureDetails()
{
    verticalLayout = new QVBoxLayout(this);
    helloWorld = new QLabel("Long Exposure");
    verticalLayout->addWidget(helloWorld);
    this->setLayout(verticalLayout);
}