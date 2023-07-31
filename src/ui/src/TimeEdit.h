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

    private slots:
        void handleTextChanged(const QString &text);

    private:

        QVBoxLayout *rootLayout;
        QHBoxLayout *lineEditLayout;
        QHBoxLayout *errorMessageLayout;
        QLabel *label;
        QLineEdit* edit;
        QLabel* errorImage;
        QLabel* errorMessage;


};