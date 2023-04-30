#include "normalization.h"

constexpr float GAMMA = 2.2f;

float common::normalization::clamp(float x) {
	if (x > 1.0) {
		return 1.0;
	}
	else if (x < 0.0) {
		return 0.0;
	}
	else {
		return x;
	}
}

float common::normalization::in_pixel(float pixel)
{
    return pow(pixel/255.0f, 1.0f/GAMMA);
}

float common::normalization::out_pixel(float pixel)
{
    return pow(common::normalization::clamp(pixel), GAMMA)*255.0f;
}

float common::normalization::get_normalized_matrix_channel(Mat& m, int x, int y, int channel_number)
{
    return common::normalization::in_pixel((float)m.at<Vec4b>(cv::Point(x, y))[channel_number]);
}