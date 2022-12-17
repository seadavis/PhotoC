#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <eigen3/Eigen/Dense>
#include <opencv2/imgcodecs.hpp>
#include <memory>
#include <string>

using namespace cv;
using namespace std;

namespace processing{

    class BackgroundResizedException : public std::exception {
        public:
            BackgroundResizedException(int originalBackgroundHeight, 
                                       int originalBackgroundWidth,
                                       int resizedWidth,
                                       int resizedHeight,
                                       int foregroundWidth,
                                       int foregroundHeight)
            {
                this->originalBackgroundWidth = originalBackgroundWidth;
                this->originalBackgroundHeight = originalBackgroundHeight;
                this->resizedWidth = resizedWidth;
                this->resizedHeight = resizedHeight;
                this->foregroundWidth = foregroundWidth;
                this->foregroundHeight = foregroundHeight;
            }

            int originalBackgroundHeight;
            int originalBackgroundWidth;
            int resizedWidth;
            int resizedHeight;
            int foregroundWidth;
            int foregroundHeight;

            const char* what () {
                auto msg = "Background Image had a width of: " + to_string(originalBackgroundWidth) + " and a height of: " + to_string(originalBackgroundHeight)
                        + "but was resized to a width of: " + to_string(resizedWidth) + " and a height of: " + to_string(resizedHeight) + " which is smaller"
                        + "than the foreground with a width of: " + to_string(foregroundWidth) + " and a height of: " + to_string(foregroundHeight);
                return msg.c_str();
            }   
    };

    class CompositeCanvas
    {

        public:

            /**
             * 
             * Initializes a canvas with 
             * the given wtih and height
            */
            CompositeCanvas(int height, int width);

            /**
             * Sets the background image for the given canvas
             * @param m - the image we want to set as a background
             *  this method will then make a copy and save the background as a unique pointer
            */
            void setBackground(Mat m);

            /**
             * Sets the composite image for the
             * given canvas.
             * 
             * @param maskImagePath - the fully qualified path to the image we want to set as the 
             * composite
             * 
             * @param originalImage - the original image that the composite was
             * cut form.
            */
            void setComposite(const string& maskImgPath, const string& originalImage);

            /**
             * 
             * Gives the current representation 
             * of the image
            */
            Mat currentImg();

        private:
           

            bool only_background_available();
            bool only_src_available();
            bool src_and_background_available();

            int height;
            int width;
            unique_ptr<Mat> originalImage;
            unique_ptr<Mat> maskImage;
            unique_ptr<Mat> backgroundImage;
    };

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
     * around src, and src is centered
     *
     * @param src source image. Assumes RGB and ignores any alpha channels
     * @param width the width of the canvas we are putting the image unto
     * @param height the height of the canvas we are putting the image unto
    */
    Mat make_canvas(Mat src, int width, int height);

    /**
     * Draws a bounding rectangle
     * on around each of the objects found with canny object
     * detection
     * 
     * @param src the image we want to draw a rectangle around. Assumes ABGR formar.
     * 
    */
    Mat bounding_rectangle(Mat& src);
}