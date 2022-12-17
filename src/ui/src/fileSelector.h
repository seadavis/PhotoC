#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QImage>
#include <QLineEdit>
#include <string>

using namespace std;

class FileSelector : public QWidget
{
    public:
        FileSelector(string& path);

    private slots:
      void selectFile();

    private:
        QHBoxLayout *mainLayout;
        QVBoxLayout *fileTextLayout;
        QLineEdit* sourceEdit;
        QLabel* statusImg;
        string& path;
        QPushButton* folderImg;
        QLabel* errorLabel;
};