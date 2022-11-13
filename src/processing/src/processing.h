#include <opencv2/core.hpp>
#include <eigen3/Eigen/Dense>

using namespace cv;


namespace processing{


    /**
     * Pastes the mask image into the target image,
     * adjusting the color using the source, which
     * is where the mask came from.
     * 
     * Assumes all images are in a BGRA format.
     * dx and dy are specified such that mask has at least one 
     * pixel between itself and the target.
     * 
     * @param mask the portion of the image we want to  
     * any fully transparent pixels will not be pasted into target
     * 
     * @param src the original image the mask came from
     * 
     * @param target the "target" we want to put the image into
     * 
     * @param dx specifies in pixel space how far horizontally
     * we want to start pasting the image, Left most pixel is 0.
     * 
     * @param dy specifies in pixel space how far vertically
     * we want to start pasting the image, upper most pixel is 0.
     */
    Mat composite(Mat mask, Mat src, Mat target, unsigned int dx, unsigned int dy);
}