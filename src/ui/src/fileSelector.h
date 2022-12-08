#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QImage>
#include <QLineEdit>

class FileSelector : public QWidget
{
    public:
        FileSelector();

    private slots:
      void selectFile();

    private:
        QHBoxLayout *mainLayout;
        QVBoxLayout *fileTextLayout;


        QLineEdit* sourceEdit;
        QLabel* statusImg;
        QPushButton* folderImg;
        QLabel* errorLabel;
};