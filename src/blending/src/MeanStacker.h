#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class MeanStacker
{

    public:
        MeanStacker();
        void AddToStack(Mat img);
        Mat GetCurrentBlend();

};