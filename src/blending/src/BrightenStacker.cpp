#include "BrightenStacker.h"
#include <cmath>
#include <algorithm>

struct PixelBrightnessMeasure
{
    int index;
    double measure;

    PixelBrightnessMeasure(int index, double measure) : index(index), measure(measure){};

    PixelBrightnessMeasure(){};
};

static double pixel_brightness(Vec4b& v)
{
    return sqrt(pow(v[0], 2) + pow(v[1], 2) + pow(v[2], 2));
}

struct less_than_key
{
    inline bool operator() (const PixelBrightnessMeasure& struct1, const PixelBrightnessMeasure& struct2)
    {
        return (struct1.measure < struct2.measure);
    }
};

static void in_place_sort(vector<PixelBrightnessMeasure>& original) {
    sort(original.begin(), original.end(), less_than_key());
}


BrightenStacker::BrightenStacker()
{
    width = -1;
    height = -1;
}

void BrightenStacker::init(Mat img)
{
    width = img.size().width;
    height = img.size().height;
    stack = vector<vector<vector<Vec4b>>>(width);

    for(int x = 0; x < width; x++)
    {
        auto columnVector = vector<vector<Vec4b>>(height);

        for(int y = 0; y < height; y++)
        {
            columnVector[y] = vector<Vec4b>();
        }

        stack[x] = columnVector;
    }
}

void BrightenStacker::AddToStack(Mat img)
{

    if(height == -1 && width == -1)
    {
        init(img);
    }

    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; y++)
        {
            
            stack[x][y].push_back(img.at<Vec4b>(cv::Point(x, y)));
            
        }
    }
}

Mat BrightenStacker::GetCurrentBlend()
{
    Mat blended = Mat(Size(width, height), CV_8UC4, Scalar(0));
    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; y++)
        {
            auto channelVector = stack[x][y];
            auto pixelMeasures = vector<PixelBrightnessMeasure>(channelVector.size());

            for(int c = 0; c < pixelMeasures.size(); c++)
            {
                pixelMeasures[c] = PixelBrightnessMeasure(c, pixel_brightness(channelVector[c]));
            }


            in_place_sort(pixelMeasures);
            double n = channelVector.size();
            blended.at<Vec4b>(cv::Point(x, y)) = channelVector[pixelMeasures[n - 1].index];
            
        }
    }

    return blended;
}
