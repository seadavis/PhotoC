#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

class LongExposureConfig : public QWidget
{
    public:
        LongExposureConfig();

    private:

        QLabel *helloWorld;
        QVBoxLayout *verticalLayout;

};