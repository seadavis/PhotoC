#include "compositeselection.h"

void CompositeSelection::setMaskPath(string path)
{
    emit maskPathChanged(path);
}

void CompositeSelection::setOriginalPath(string path)
{
    emit originalPathChanged(path);
}


void CompositeSelection::resizeEvent(QResizeEvent* event) 
{
    groupBox->setFixedHeight(this->height()*0.25);
    groupBox->setFixedWidth(this->width()*0.95);
}

CompositeSelection::CompositeSelection()
{
    vBoxLayout = new QVBoxLayout;
    groupBox = new QGroupBox(this);
    maskSelector = new FileSelector;
    originalSelector = new FileSelector;
    maskSelector->setTitle("Mask:");
    originalSelector->setTitle("Original:");

    vBoxLayout->addWidget(maskSelector);
    vBoxLayout->addWidget(originalSelector);
    groupBox->setLayout(vBoxLayout);

    connect(maskSelector, &FileSelector::fileChanged, this, &CompositeSelection::setMaskPath);
    connect(originalSelector, &FileSelector::fileChanged, this, &CompositeSelection::setOriginalPath);
}