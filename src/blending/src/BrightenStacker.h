#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

typedef vector<vector<double>> intensity_array;

class BrightenStacker
{

    public:
        BrightenStacker();
        void AddToStack(Mat& img);
        Mat GetCurrentBlend();

    private:
        int width;
        int height;
        Mat currentBlend;
        intensity_array currentIntensities;
        int Index(int x, int y);
        void init(Mat&);

};