#include "MedianStacker.h"
#include <cmath>
#include <algorithm>

static vector<uchar> not_in_place_sort(vector<uchar> original) {
    sort(original.begin(), original.end());
    return original;
}

MedianStacker::MedianStacker(int width, int height)
{
    this->width = width;
    this->height = height;
    seperatedMat = vector<vector<SeperatedChannels>>();

    for(int x = 0; x < width; x++)
    {
        auto columnVector = vector<SeperatedChannels>();
        for(int y = 0; y < height; y++)
        {
            SeperatedChannels channelVector;

            for(int c = 0; c < NumberOfChannels; c++)
            {
                channelVector[c] = vector<uchar>();
            }

            columnVector.push_back(channelVector);
        }
        seperatedMat.push_back(columnVector);
    }
}

void MedianStacker::AddToStack(Mat img)
{
    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; y++)
        {
            for(int c = 0; c < NumberOfChannels; c++)
            {
                seperatedMat[x][y][c].push_back(img.at<Vec4b>(cv::Point(x, y))[c]);
            }
        }
    }
}

Mat MedianStacker::GetCurrentBlend()
{
    Mat blended = Mat(Size(width, height), CV_8UC4, Scalar(0));
    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; y++)
        {
            auto channelVector = seperatedMat[x][y];
            for(int c = 0; c < NumberOfChannels; c++)
            {
                auto pixelValues = channelVector[c];
                vector<uchar> sortedPixels = not_in_place_sort(pixelValues);
                double n = pixelValues.size();
                int medianIndex = ceil(n/2.0);
                blended.at<Vec4b>(cv::Point(x, y))[c] = sortedPixels[medianIndex];
            }
        }
    }

    return blended;
}
