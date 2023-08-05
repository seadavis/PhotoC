#include <opencv2/opencv.hpp>
#include <vector>
#pragma once

using namespace cv;
using namespace std;

class IStackImages
{
    public:
        virtual void AddToStack(Mat& img) = 0;
        virtual Mat GetCurrentBlend() = 0;
};

double pixel_brightness(Vec4b& v);

double pixel_brightness(uchar r, uchar g, uchar b);

class StackResizedException : public std::exception {
        public:
            StackResizedException(int originalBackgroundHeight, 
                                       int originalBackgroundWidth,
                                       int resizedWidth,
                                       int resizedHeight)
            {
                this->originalBackgroundWidth = originalBackgroundWidth;
                this->originalBackgroundHeight = originalBackgroundHeight;
                this->resizedWidth = resizedWidth;
                this->resizedHeight = resizedHeight;
            }

            int originalBackgroundHeight;
            int originalBackgroundWidth;
            int resizedWidth;
            int resizedHeight;

            const char* what () {
                auto msg = "Original Stack Image had a width of: " + to_string(originalBackgroundWidth) + " and a height of: " + to_string(originalBackgroundHeight)
                        + "but second image had a width of: " + to_string(resizedWidth) + " and a height of: " + to_string(resizedHeight);
                return msg.c_str();
            }   
    };


class TypeMismatchException : public std::exception{

    public:
        TypeMismatchException(){}

    const char* what()
    {
        return "Image format does not match the format initially added to the stack.";
    }

};