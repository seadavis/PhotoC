#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

constexpr int NumberOfChannels = 4;

typedef array<vector<uchar>, NumberOfChannels> SeperatedChannels;

class MedianStacker
{

    public:
        MedianStacker(int width, int height);
        void AddToStack(Mat img);
        Mat GetCurrentBlend();

    private:
        vector<vector<SeperatedChannels>> seperatedMat;
        int width;
        int height;
        int Index(int x, int y);

};