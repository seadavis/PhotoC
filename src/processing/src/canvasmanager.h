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
            virtual Mat Operate(CompositeCanvas& canvas) = 0;
    };

    class CompositeImageUpdate : ICanvasOperator
    {
        public:
            CompositeImageUpdate(string originalImagePath, string maskPath) : 
                originalImagePath(originalImagePath), maskPath(maskPath)
            {};

            string originalImagePath;

            string maskPath;

            Mat Operate(CompositeCanvas& canvas) override;

    };

    class BackgroundImageUpdate : ICanvasOperator
    {
        public:
            BackgroundImageUpdate(Mat& backgroundImage) :
                backgroundImage(backgroundImage)
            {};

            Mat& backgroundImage;

            Mat Operate(CompositeCanvas& canvas) override;
    };

    /**
     * Responsible for managing operations into the canvas
    */
    class CanvasManager
    {

        public:
            CanvasManager(IRenderImages* renderer);
            void QueueOperation(ICanvasOperator& op);


        private:
            unique_ptr<CompositeCanvas> canvas;
            IRenderImages* renderer;

    };

}



