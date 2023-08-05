#include <QWidget>
#include <QErrorMessage>
#include <exception>

void showErrorMessage(QWidget* parent, const std::exception& ex);