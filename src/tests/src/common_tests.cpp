#include "common_tests.h"

Mat common::tests::loadStdImage(string path)
{
  auto backgroundImageRGB = imread(path, CV_LOAD_IMAGE_UNCHANGED);
  Mat backgroundImage;
  cvtColor(backgroundImageRGB, backgroundImage, CV_BGR2BGRA);
  return backgroundImage;
}

bool common::tests::compareImages(const string& expectedImagePath, 
                                  const string& actualImagePath,
                                  Mat& actualImage)
{
  imwrite(actualImagePath, actualImage);
  Mat expectedImg = imread(expectedImagePath, CV_LOAD_IMAGE_UNCHANGED);
  return std::equal(actualImage.begin<uchar>(), actualImage.end<uchar>(), expectedImg.begin<uchar>()); 
}