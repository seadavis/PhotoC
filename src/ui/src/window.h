#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

class Window : public QWidget
{

  Q_OBJECT

 public:
  
  explicit Window(QWidget *parent = 0);


 private slots:
    void handleButton();

 private:
    QLabel *label;
    QPixmap *pixmap;
    QImage *image;
    QPushButton *button; 
};

#endif // WINDOW_H