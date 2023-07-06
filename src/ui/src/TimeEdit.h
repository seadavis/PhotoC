#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QString>

class TimeEdit : public QWidget
{
    public:
        TimeEdit(QString label);

    private:

        QHBoxLayout *layout;
        QLabel *label;
        QLineEdit* edit;
};