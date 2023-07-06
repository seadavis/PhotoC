#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

class LongExposureDetails : public QWidget
{
    public:
        LongExposureDetails();

    private:

        QLabel *helloWorld;
        QVBoxLayout *verticalLayout;

};