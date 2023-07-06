#include "TimeEdit.h"

TimeEdit::TimeEdit(QString label)
{
    layout = new QHBoxLayout;
    this->label = new QLabel(label);
    edit = new QLineEdit("hh:mm:ss");
    layout->addWidget(this->label);
    layout->addWidget(edit);

    this->setLayout(layout);
}