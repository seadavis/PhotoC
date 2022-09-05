#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

class Window : public QWidget
{
 public:
  explicit Window(QWidget *parent = 0);
 private:
    QLabel *label;
    QPixmap *pixmap;
    QImage *image;
};

#endif // WINDOW_H