#include "processing.h"

using namespace std;

namespace processing{

    /**
    * Interface used to Render images
    */
    class IRenderImages
    {
        public:
            virtual void RenderImage(Mat& m) = 0;
    };

    class ICanvasOperator
    {
        public:
            virtual void Operate(CompositeCanvas& canvas) = 0;
    };

    class CompositeImageUpdate : public ICanvasOperator
    {
        public:
            CompositeImageUpdate(string originalImagePath, string maskPath) : 
                originalImagePath(originalImagePath), maskPath(maskPath)
            {};

            string originalImagePath;

            string maskPath;

            void Operate(CompositeCanvas& canvas) override;

    };

    class BackgroundImageUpdate : public ICanvasOperator
    {
        public:
            BackgroundImageUpdate(Mat& backgroundImage) :
                backgroundImage(backgroundImage)
            {};

            Mat& backgroundImage;

            void Operate(CompositeCanvas& canvas) override;
    };

    class Resize : public ICanvasOperator
    {
           public:
            Resize(int width, int height) :
                width(width), height(height)
            {};

            int width;
            int height;

            void Operate(CompositeCanvas& canvas) override;
    };

    /**
     * Responsible for managing operations into the canvas
    */
    class CanvasManager
    {

        public:
            CanvasManager(CompositeCanvas* canvas, IRenderImages* renderer);
            void QueueOperation(ICanvasOperator& op);


        private:
            CompositeCanvas* canvas;
            IRenderImages* renderer;

    };

}



