#include "compositeselection.h"


CompositeSelection::CompositeSelection()
{
    layout = new QVBoxLayout(this);
    sourceSelector = new FileSelector;
    targetSelector = new FileSelector;
    layout->addWidget(sourceSelector);
    layout->addWidget(targetSelector);
}