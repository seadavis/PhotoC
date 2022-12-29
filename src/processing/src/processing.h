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

            /***
             * Tells that canvas that a user
             * has "tapped" the given point. The point
             * should be in corrdinates relative to canvas space.
             * 
             * Selects an image if one is to be selected.
             * 
             * @param p - the point that was tapped.
            */
            void tap(Point p);

            /**
             * Determines if the composite image is hit at that particular point or not
             * 
             * @param p - the point we are querying - in canvas space
            */
            bool hit(Point p);


            /**
             * 
             * Horizontally and vertically translates the last 
             * selected image. If no composite was selected then 
             * we do nothing
             * 
             * @param dx - the amount to horizontally translate the last selected
             * image. Relative to the entire canvas
             * 
             * @param dy - the amount to vertically translate the last selected
             * image.
            */
            void translate(int dx, int dy);


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
            Rect translate_to_canvas_coordindates(const Rect& r);
            Mat loadImage(string imagePath);

            void initPlacement();

            void draw_adornments(Mat canvas);

            bool showBoundingRectangle = false;

            int height;
            int width;

            // position of image in image coordinates
            int mx;
            int my;

            unique_ptr<Mat> originalImage;
            unique_ptr<Mat> maskImage;
            unique_ptr<Mat> backgroundImage;
            Rect boundingRectangle;
    };
}