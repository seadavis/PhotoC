#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QImage>
#include <QLabel>
#include <QPixmap>

class Window : public QWidget
{
 public:
  explicit Window(QWidget *parent = 0);
 private:
    QImage *image;
    QLabel *label;

};

#endif // WINDOW_H