#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include "blending_common.h"

using namespace cv;
using namespace std;


class MeanStacker : public IStackImages
{

    public:
        MeanStacker();
        void AddToStack(Mat& img) override;
        Mat GetCurrentBlend() override;

    private:
        vector<Mat> stack;

};