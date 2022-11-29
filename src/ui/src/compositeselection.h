#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QImage>

class CompositeSelection : public QWidget
{
    public:
        CompositeSelection();

    private:
        QVBoxLayout* layout;
        QLabel *label;
};