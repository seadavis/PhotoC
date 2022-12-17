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
        CompositeSelection(shared_ptr<CompositePaths> paths);

    private:
        QVBoxLayout* layout;
        shared_ptr<CompositePaths> paths;
        FileSelector* sourceSelector;
        FileSelector* targetSelector;

};