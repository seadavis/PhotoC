#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Sparse>
#include <opencv2/imgcodecs.hpp>
#include <memory>
#include <string>
#include "normalization.h"

using namespace std;
using namespace Eigen;

typedef SparseMatrix<float> SpMat;
typedef Eigen::Triplet<float> MatTriplet;
typedef Eigen::SimplicialLDLT<SpMat> Solver;

namespace processing{

    enum ObjectType
    {
        Image,
        SizeCircle,
        None
    };

    class Circle
    {
        public:
            Circle(int radius, Point center) :
                radius(radius), center(center)
                {}

            Circle() :
                radius(0), center(Point(0, 0))
            {}

            int radius;
            Point center;

            void draw(Mat m);

    };

    /**
     * used internally to processing to represent a border
     * around an "Image", such as a mask
     * used in a composite
    */
    class ImageBorder
    {
        public:

            /**
             * @param width - the width of the image border not including 
             * the "resizing circles"
             * 
             * @param height - the height of the image border not including
             * the "resizing circles"
             * 
             * @param tl - the top left of the image border.
            */
            ImageBorder(int width, int height, Point tl);

            ImageBorder(){};

            /**
             * 
             * Draws intself unto the given matrix
            */
            void draw(Mat m);

        
            int width();

            int height();

            ObjectType hit(Point p);

            /**
             * 
             * The top left point of the border
            */
            Point tl();

            /**
             * The bottom right point of the border
            */
            Point br();

        private:
            Rect r;
            Circle tl_circle;
            Circle br_circle;

    };

    
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
            ObjectType hit(Point p);

            /**
             * 
             * Tells us the cursorMoved 
             * by the given change in x and change in y. Both measure in pixels
             * 
             * @param dx - the amount the cursor moved horizontally
             * 
             * @param dy - the amount the cursor moved vertically
            */
            void translateSelected(int dx, int dy);

            /**
             * Scales whichever image is currently selected.
            */
            void scaleSelected(int dx, int dy);

            /**
             * Releases the currently held object if it
             * exists. If no object is being held this does nothing.
             * 
            */
            void releaseObject();

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
            bool mask_and_original_available();
            void setSupportingStructuresForComposites();
            ImageBorder translate_to_canvas_coordindates(ImageBorder b);
            Mat loadImage(string imagePath);

            void alignMaskSize();

            Solver solver;
            map<unsigned int, unsigned int> variableMap;

            void initPlacement();

            void draw_adornments(Mat canvas);

            bool showBoundingRectangle = false;
            ObjectType objectSelected = ObjectType::None;

            int height;
            int width;

            int maskWidth;
            int maskHeight;

            // position of image in image coordinates
            int mx;
            int my;

            // the scaling of x and Y repsectively
            int scaleX = 1;
            int scaleY = 1;

            unique_ptr<Mat> originalImage;
            unique_ptr<Mat> originalMaskImage;
            unique_ptr<Mat> resizedMask;
            unique_ptr<Mat> resizedOriginal;

            unique_ptr<Mat> backgroundImage;
            ImageBorder border;
    };
}