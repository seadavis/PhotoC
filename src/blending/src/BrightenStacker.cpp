#include "BrightenStacker.h"
#include <cmath>
#include <algorithm>
#include "common.h"

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
}

void BrightenStacker::AddToStack(Mat& img)
{

    if(height == -1 && width == -1)
    {
        init(img);
    }

    for(int y = 0; y < height; y++)
    {
        uchar* incomingRowPtr = img.ptr(y);
        uchar* currentRowPtr = currentBlend.ptr(y);

        /**
         * 
         * Comparing the incoming and current array seperately helps with cache usage
        */
        double* incomingIntensities = (double*)malloc(width*sizeof(double));
        double* currentIntensities = (double*)malloc(width*sizeof(double));

        int currentPixel = 0;
        for(int x = 0; x < width*4; x+= 4)
        {
            auto incomingR = incomingRowPtr[x];
            auto incomingG = incomingRowPtr[x+1];
            auto incomingB = incomingRowPtr[x+2];

            incomingIntensities[currentPixel] = pixel_brightness(incomingR, incomingG, incomingB);
            currentPixel++;
        }

        currentPixel = 0;
        for(int x = 0; x < width*4; x+= 4)
        {
            auto currentR = currentRowPtr[x];
            auto currentG = currentRowPtr[x+1];
            auto currentB = currentRowPtr[x+2];

            currentIntensities[currentPixel] = pixel_brightness(currentR, currentG, currentB);
            currentPixel++;
        }

        for(int x = 0; x < width; x++)
        {
            if(currentIntensities[x] < incomingIntensities[x])
            {
                auto p = x*4;
                currentRowPtr[p] = incomingRowPtr[p];
                currentRowPtr[p+1] = incomingRowPtr[p+1];
                currentRowPtr[p+2] = incomingRowPtr[p+2];
                currentRowPtr[p+3] = incomingRowPtr[p+3];
            }
        }

        free(incomingIntensities);
        free(currentIntensities);
    }
}

Mat BrightenStacker::GetCurrentBlend()
{
   return currentBlend.clone();
}
