#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <cmath>
#pragma once

using namespace cv;
using namespace std;

namespace common::normalization
{
    float clamp(float x);

    float in_pixel(float pixel);

    float out_pixel(float pixel);

    float get_normalized_matrix_channel(Mat& m, int x, int y, int channel_number);
}
