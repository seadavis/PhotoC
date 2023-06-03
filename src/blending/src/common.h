#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

struct PixelBrightnessMeasure
{
    int index;
    double measure;

    PixelBrightnessMeasure(int index, double measure) : index(index), measure(measure){};

    PixelBrightnessMeasure(){};
};

struct less_than_key
{
    inline bool operator() (const PixelBrightnessMeasure& struct1, const PixelBrightnessMeasure& struct2)
    {
        return (struct1.measure < struct2.measure);
    }
};

double pixel_brightness(Vec4b& v);

void in_place_sort(vector<PixelBrightnessMeasure>& original);