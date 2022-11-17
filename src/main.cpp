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
#include "window.h"

int main(int argc, char **argv)
{



    QApplication app (argc, argv);

    Window window;
    window.show();

    return app.exec();
}