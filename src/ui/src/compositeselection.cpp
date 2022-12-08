#include "compositeselection.h"


CompositeSelection::CompositeSelection()
{
    layout = new QVBoxLayout(this);
    sourceSelector = new FileSelector;
    layout->addWidget(sourceSelector);
}