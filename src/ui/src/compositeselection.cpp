#include "compositeselection.h"

CompositeSelection::CompositeSelection()
{

    layout = new QVBoxLayout(this);

    
    label = new QLabel;
    label->setText("Hello");
    layout->addWidget(label);
}