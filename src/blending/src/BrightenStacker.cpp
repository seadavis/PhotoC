#include "BrightenStacker.h"
#include <cmath>
#include <algorithm>
#include <future>
#include <cmath>


/**
 * Compares current against incoming. And where incoming 
 * has a higher intensity replaces it. Only works on startRow
 * to endRow (incluseive.)
 * 
 * @param currentBlend - the current blend used to compare against the intensities
 * @param incoming - the incoming image to compare the current against
 * @param currentIntensities - the current set of intensities. This will be updated
 * at the end of operation
 * 
 * @param incomingIntensities - the array for holding the intensities on the image.
 * Will be filled by this algorithm
 * 
 * @param startRow - 0-indexed start of the calculations (inclusive)
 * @param endRow 0-indexed end of the calculations (inclusive)
*/
static void compareIntensities(Mat* currentBlend, 
                                Mat* incoming, 
                                intensity_array* currentIntensities, 
                                intensity_array* incomingIntensities, 
                                int startRow, 
                                int endRow);

static void fillIntensities(intensity_array* intensities, Mat& img, int startRow, int endRow);

BrightenStacker::BrightenStacker()
{
    width = -1;
    height = -1;
}

void BrightenStacker::init(Mat& img)
{
    width = img.size().width;
    height = img.size().height;
    currentBlend = img.clone();
    currentIntensities = vector<vector<double>>(height, vector<double>(width));
    fillIntensities(&currentIntensities, currentBlend, 0, height - 1);

    int heightDivision = height/3;

    startIndex1 = 0;
    endIndex1 = heightDivision - 1;

    startIndex2 = endIndex1 + 1;
    endIndex2 = startIndex2 + (heightDivision - 1);

    startIndex3 = endIndex2 + 1;
    endIndex3 = height - 1; 

}

void BrightenStacker::AddToStack(Mat& img)
{

    if(height == -1 && width == -1)
    {
        init(img);
    }
    else
    {
        if(img.size().height != height || img.size().width != width)
        {
            throw StackResizedException(height, width, img.size().width, img.size().height);
        }

        if(img.type() != currentBlend.type())
        {
            throw TypeMismatchException();
        }

        auto incomingIntensities = vector<vector<double>>(height, vector<double>(width));

        auto t1 = async(compareIntensities, &currentBlend, &img, &currentIntensities, &incomingIntensities, startIndex1, endIndex1);
        auto t2 = async(compareIntensities, &currentBlend, &img, &currentIntensities, &incomingIntensities, startIndex2, endIndex2);
        auto t3 = async(compareIntensities, &currentBlend, &img, &currentIntensities, &incomingIntensities, startIndex3, endIndex3);

        t1.wait();
        t2.wait();
        t3.wait();
    }
}

Mat BrightenStacker::GetCurrentBlend()
{
   return currentBlend.clone();
}

static void compareIntensities(Mat* currentBlend, 
                                Mat* incoming, 
                                intensity_array* currentIntensities, 
                                intensity_array* incomingIntensities, 
                                int startRow, 
                                int endRow)
{

    auto size = incoming->size();
    auto height = size.height;
    auto width = size.width;


    fillIntensities(incomingIntensities, *incoming, startRow, endRow);

    for(int y = startRow; y <= endRow; y++)
    {
        for(int x = 0; x < width; x++)
        {
            if((*currentIntensities)[y][x] < (*incomingIntensities)[y][x])
            {
                uchar* incomingRowPtr = incoming->ptr(y);
                uchar* currentRowPtr = currentBlend->ptr(y);

                auto p = x*4;
                currentRowPtr[p] = incomingRowPtr[p];
                currentRowPtr[p+1] = incomingRowPtr[p+1];
                currentRowPtr[p+2] = incomingRowPtr[p+2];
                currentRowPtr[p+3] = incomingRowPtr[p+3];
                (*currentIntensities)[y][x] = (*incomingIntensities)[y][x];
            }
        }
    }

}

static void fillIntensities(intensity_array* intensities, Mat& img, int startRow, int endRow)
{
    for(int y = startRow; y <= endRow; y++)
    {
        uchar* rowPtr = img.ptr(y);

        int currentPixel = 0;
        for(int x = 0; x < img.size().width*4; x+= 4)
        {
            auto incomingR = rowPtr[x];
            auto incomingG = rowPtr[x+1];
            auto incomingB = rowPtr[x+2];

            (*intensities)[y][currentPixel] = pixel_brightness(incomingR, incomingG, incomingB);
            currentPixel++;
        }
    }
}