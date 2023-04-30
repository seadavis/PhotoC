#include "MeanStacker.h"

MeanStacker::MeanStacker()
{

}

void MeanStacker::AddToStack(Mat img)
{
    Mat copy;
    img.copyTo(copy);
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