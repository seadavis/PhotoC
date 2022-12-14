#include <gtest/gtest.h>
#include "processing.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <opencv2/highgui.hpp>

using namespace std;

class Canvas :
    public testing::TestWithParam<tuple<int, int, int, int>> {
};

INSTANTIATE_TEST_SUITE_P(CanvasTests,
                         Canvas,
                         testing::Values(

                          make_tuple(1300, 1300, 5, 5),
                          make_tuple(250, 250,10, 10),
                          make_tuple(1300, 250,10, 10),
                            make_tuple(894,596,50, 50)
                          
                        ));

TEST_P(Canvas, MakeCanvas) {

  auto args = GetParam();
  Mat m = imread("./src/processing/tests/target_images/beach.png", IMREAD_UNCHANGED);

  Mat tgt;
  cvtColor(m, tgt, CV_BGR2BGRA);

 
  Mat actualMat = processing::make_canvas(tgt, get<0>(args), get<1>(args));
  auto expectedFile = "./src/processing/tests/target_canvas/" + to_string(get<0>(args)) + "_" + to_string(get<1>(args)) + "_" + to_string(get<2>(args)) + "_" + to_string(get<3>(args)) +".png";
  Mat expectedMat = imread(expectedFile);

  bool const equal = std::equal(actualMat.begin<uchar>(), actualMat.end<uchar>(), expectedMat.begin<uchar>());
  ASSERT_TRUE(equal);
}

TEST(CompositeCanvas, BoundingRect)
{
    Mat m = imread("./src/processing/tests/masks/eagle.png", IMREAD_UNCHANGED);

    Mat tgt;
    cvtColor(m, tgt, CV_BGR2BGRA);

    Mat result = processing::bounding_rectangle(tgt);

    imwrite("./test.png", result);

}