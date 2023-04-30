#include "loaders.h"

Mat common::tests::loadStdImage(string path)
{
  auto backgroundImageRGB = imread(path, CV_LOAD_IMAGE_UNCHANGED);
  Mat backgroundImage;
  cvtColor(backgroundImageRGB, backgroundImage, CV_BGR2BGRA);
  return backgroundImage;
}