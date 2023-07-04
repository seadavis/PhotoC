#include "BrightenStacker.h"
#include <cmath>
#include <algorithm>
#include "common.h"

static void fillIntensities(intensity_array& intensities, Mat& img);

BrightenStacker::BrightenStacker()
{
    width = -1;
    height = -1;
}

void BrightenStacker::init(Mat& img)
{
    width = img.size().width;
    height = img.size().height;
    currentBlend = Mat(img.size(), img.type());
    currentBlend.setTo(Scalar(0, 0, 0));
    currentIntensities = vector<vector<double>>(height, vector<double>(width));
    fillIntensities(currentIntensities, currentBlend);
}

void BrightenStacker::AddToStack(Mat& img)
{

    if(height == -1 && width == -1)
    {
        init(img);
    }

    auto incomingIntensities = vector<vector<double>>(height, vector<double>(width));
    fillIntensities(incomingIntensities, img);

    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            if(currentIntensities[y][x] < incomingIntensities[y][x])
            {
                uchar* incomingRowPtr = img.ptr(y);
                uchar* currentRowPtr = currentBlend.ptr(y);
                
                auto p = x*4;
                currentRowPtr[p] = incomingRowPtr[p];
                currentRowPtr[p+1] = incomingRowPtr[p+1];
                currentRowPtr[p+2] = incomingRowPtr[p+2];
                currentRowPtr[p+3] = incomingRowPtr[p+3];
                currentIntensities[y][x] = incomingIntensities[y][x];
            }
        }
    }
}


static void fillIntensities(intensity_array& intensities, Mat& img)
{
    for(int y = 0; y < img.size().height; y++)
    {
        uchar* rowPtr = img.ptr(y);

        int currentPixel = 0;
        for(int x = 0; x < img.size().width*4; x+= 4)
        {
            auto incomingR = rowPtr[x];
            auto incomingG = rowPtr[x+1];
            auto incomingB = rowPtr[x+2];

            intensities[y][currentPixel] = pixel_brightness(incomingR, incomingG, incomingB);
            currentPixel++;
        }
    }
}


Mat BrightenStacker::GetCurrentBlend()
{
   return currentBlend.clone();
}
