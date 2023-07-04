#include "MeanStacker.h"
#include "common.h"

MeanStacker::MeanStacker()
{

}

void MeanStacker::AddToStack(Mat img)
{
    Mat copy;
    img.copyTo(copy);
    
    if(stack.size() > 0)
    {
        auto imgSize = img.size();
        auto initSize = stack[0].size();

        if(imgSize.height != initSize.height || imgSize.width != initSize.width)
        {
            throw StackResizedException(initSize.height, initSize.width, imgSize.width, imgSize.height);
        }

        if(img.type() != stack[0].type())
        {
            throw TypeMismatchException();
        }
    }

    stack.push_back(img);
}

Mat MeanStacker::GetCurrentBlend()
{
    const double n = stack.size();

    Mat accumulator = Mat(stack[0].size(), stack[0].type(), Scalar(0)); ;
    for(int i = 0; i < n; i++)
    {
        accumulator = accumulator + (1.0/n)*stack[i];
    }

    return accumulator;
}