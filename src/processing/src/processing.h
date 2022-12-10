#include <opencv2/core.hpp>
#include<opencv2/opencv.hpp>
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

    /**
     * makes a new mat that fits within width and height
     * and plus the two padding parameters. If src is 
     * smaller than the width or the height black pixels are added
     * around src.
     *
     * @param src source image. Assumes RGB and ignores any alpha channels
     * @param width the width of the canvas we are putting the image unto
     * @param height the height of the canvas we are putting the image unto
     * @param width_padding the padding added to the image on the canvas
     * @param height_padding the padding added to the image on the canvas
    */
    Mat make_canvas(Mat src, int width, int height, int width_padding, int height_padding);
}