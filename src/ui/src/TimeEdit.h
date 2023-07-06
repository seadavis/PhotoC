#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QString>
#include "camera.h"

class TimeEdit : public QWidget
{
    public:
        TimeEdit(QString label);
        void Reset();

        TimeLength GetTimeLength();

    private:

        QHBoxLayout *layout;
        QLabel *label;
        QLineEdit* edit;
};