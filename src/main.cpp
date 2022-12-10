/*
 * This code released into the public domain 21 July 2008
 *
 * This program does the equivalent of:
 * gphoto2 --shell
 *   > set-config capture=1
 *   > capture-image-and-download
 * compile with gcc -Wall -o canon-capture -lgphoto2 canon-capture.c
 *
 * Taken from: http://credentiality2.blogspot.com/2008/07/linux-libgphoto2-image-capture-from.html
 *
 * and condensed into simple capture sample
 */

#include <QApplication>
#include <QFile>
#include "mainwindow.h"

int main(int argc, char **argv)
{
    QApplication app (argc, argv);
   
    QFile file(":/resources/stylesheets/default.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    app.setStyleSheet(styleSheet);

    ICamera* camera;
    if(argc > 1)
    {   
        
        camera = new FakeCamera(string(argv[1]));
    }
    else
    {
        camera = new RemoteCamera();
    }

    MainWindow window(camera);
    
    window.setWindowState(Qt::WindowMaximized);
    window.setWindowFlags(Qt::Window |
                             Qt::WindowMinimizeButtonHint | 
                             Qt::WindowMaximizeButtonHint | 
                             Qt::WindowCloseButtonHint);
    window.show();

    int return_num = app.exec();

    delete camera;
    return return_num;
}