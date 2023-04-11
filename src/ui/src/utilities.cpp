#include "utilities.h"

void showErrorMessage(QWidget* parent, const std::exception& ex)
{
    auto msg = new QErrorMessage(parent);
    auto what = ex.what();
    msg->showMessage(QString(what));
}
