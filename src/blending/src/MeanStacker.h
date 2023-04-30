#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;


class MeanStacker
{

    public:
        MeanStacker();
        void AddToStack(Mat img);
        Mat GetCurrentBlend();

    private:
        vector<Mat> stack;

};