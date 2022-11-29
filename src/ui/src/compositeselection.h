#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QImage>
#include <QLineEdit>

class CompositeSelection : public QWidget
{
    public:
        CompositeSelection();

    private:
        QVBoxLayout* layout;
        QVBoxLayout* sourceLayout;

        QLabel* sourceLabel;
        QLineEdit* sourceEdit;
        QWidget* frame;
        

};