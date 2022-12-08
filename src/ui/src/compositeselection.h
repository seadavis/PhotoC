#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QImage>
#include <QLineEdit>
#include "fileSelector.h"

class CompositeSelection : public QWidget
{
    public:
        CompositeSelection();

    private:
        QVBoxLayout* layout;
        FileSelector* sourceSelector;

};