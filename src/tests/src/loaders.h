#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

namespace common::tests
{
    /**
     * Loads an image 
     * such that it is normalized to
     * bgra format
     * 
    */
    Mat loadBackgroundImage(string path);
}
