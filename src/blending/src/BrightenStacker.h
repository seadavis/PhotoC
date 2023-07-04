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

        /* Start and end indices for each thread that works during a stack*/
        int startIndex1;
        int endIndex1;

        int startIndex2;
        int endIndex2;

        int startIndex3;
        int endIndex3;

};