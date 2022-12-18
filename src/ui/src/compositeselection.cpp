#include "compositeselection.h"

void CompositeSelection::setMaskPath(string path)
{
    emit maskPathChanged(path);
}

void CompositeSelection::setOriginalPath(string path)
{
    emit originalPathChanged(path);
}

CompositeSelection::CompositeSelection()
{
    layout = new QVBoxLayout(this);

    sourceLayout = new QVBoxLayout(this);
    targetLayout = new QVBoxLayout(this);

    sourceLabel = new QLabel;
    targetLabel = new QLabel;

    sourceLabel->setText("Mask File:");
    targetLabel->setText("Original File:");

    sourceSelector = new FileSelector;
    targetSelector = new FileSelector;

    sourceLayout->addWidget(sourceLabel, 0, Qt::AlignTop);
    sourceLayout->addWidget(sourceSelector,0, Qt::AlignTop);

    targetLayout->addWidget(targetLabel,0, Qt::AlignTop);
    targetLayout->addWidget(targetSelector,0, Qt::AlignTop);
   
    layout->addLayout(sourceLayout);
    layout->addLayout(targetLayout);

    connect(sourceSelector, &FileSelector::fileChanged, this, &CompositeSelection::setMaskPath);
    connect(targetSelector, &FileSelector::fileChanged, this, &CompositeSelection::setOriginalPath);
}