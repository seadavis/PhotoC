#include "LongExposureConfig.h"

LongExposureConfig::LongExposureConfig()
{
    verticalLayout = new QVBoxLayout(this);
    helloWorld = new QLabel("hello world");
    verticalLayout->addWidget(helloWorld);
    this->setLayout(verticalLayout);
    
}