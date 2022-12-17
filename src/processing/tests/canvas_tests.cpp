#include <gtest/gtest.h>
#include "processing.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <opencv2/highgui.hpp>

using namespace std;


TEST(CompositeCanvas, BoundingRect)
{
    Mat m = imread("./src/processing/tests/masks/eagle.png", IMREAD_UNCHANGED);

    Mat tgt;
    cvtColor(m, tgt, CV_BGR2BGRA);

    Mat result = processing::bounding_rectangle(tgt);

    imwrite("./test.png", result);

}