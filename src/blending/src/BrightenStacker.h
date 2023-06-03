#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

class BrightenStacker
{

    public:
        BrightenStacker();
        void AddToStack(Mat img);
        Mat GetCurrentBlend();

    private:
        vector<vector<vector<Vec4b>>> stack;
        int width;
        int height;
        int Index(int x, int y);
        void init(Mat);

};