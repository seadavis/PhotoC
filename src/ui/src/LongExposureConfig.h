#include <QPushButton>
#include "TimeEdit.h"

class LongExposureConfig : public QWidget
{
    public:
        LongExposureConfig();

    private:

        TimeEdit* intervalEdit;
        TimeEdit* lengthEdit;
        QVBoxLayout* mainLayout;

        QHBoxLayout* buttonLayout;
        QPushButton* okButton;
        QPushButton* cancelButton;
};