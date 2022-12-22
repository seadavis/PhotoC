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

        
            CompositeCanvas();

            /**
             * Sets the background image for the given canvas
             * @param m - the image we want to set as a background
             *  this method will then make a copy and save the background as a unique pointer
            */
            void setBackground(Mat m);

            /**
             * Adjusts the total available size of the canvas
            */
            void setSize(int width, int height);

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

            void tap(Point p);

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
            Mat LoadImage(string imagePath);

            bool showBoundingRectangle;

            int height;
            int width;
            unique_ptr<Mat> originalImage;
            unique_ptr<Mat> maskImage;
            unique_ptr<Mat> backgroundImage;
            Rect boundingRectangle;

            /* the position the rectangle is drawn in */
            int x_pos = -1;
            int y_pos = -1;
    };
}