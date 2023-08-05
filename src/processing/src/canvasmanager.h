#include "processing.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>

typedef std::chrono::_V2::system_clock::time_point time_point;

using namespace std;

namespace processing{

    /**
    * Interface used to Render images
    */
    class IRenderImages
    {
        public:
            virtual void RenderImage(Mat& m) = 0;

            /**
             * Used by canvas Manager that rendering
             * has started.
            */
            virtual void RenderStarted() = 0;

            /**
             * Used by canvas manager
             * that rendering has stopped
            */
            virtual void RenderStopped() = 0;
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

    class TapImage : public ICanvasOperator
    {
        public:
            TapImage(cv::Point pointToTap) : 
                pointToTap(pointToTap)
            {};

            cv::Point pointToTap;
            void Operate(CompositeCanvas& canvas) override;

    };

    /**
     * Represents an image transformation (Scaling or translation)
     * Represented by a focal point of the transformation (i.e. where the mouse clicked)
     * and a deltax, and deltay how much the mouse move.
     * 
     * This is what determines whether it is a scale
     * or a translation
    */
    class TransformImage : public ICanvasOperator
    {
        public:
            TransformImage(cv::Point focalPoint, int dx, int dy) : 
                focalPoint(focalPoint),
                dx(dx),
                dy(dy)
            {};
            
            cv::Point focalPoint;
            int dx;
            int dy;
            virtual void OnHit(ObjectType type) {};
            void Operate(CompositeCanvas& canvas) override;

    };

    /**
     * Used to release
     * the currently selected image
     * on the canvas
    */
    class ReleaseImage : public ICanvasOperator
    {
        public:
            ReleaseImage() {};
            void Operate(CompositeCanvas& canvas) override;
    };


    class BackgroundImageUpdate : public ICanvasOperator
    {
        /**
         * When passed in a mat, assumes 
         * that Mat does not actually own the data.
         * Which it doesn't since the mat comes from
         * the camera.
         * 
         * So this manually destructs the data on destruction
        */
        public:
            BackgroundImageUpdate(Mat backgroundImage) :
                backgroundImage(backgroundImage)
            {};

            Mat backgroundImage;

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
            CanvasManager(shared_ptr<CompositeCanvas> canvas, shared_ptr<IRenderImages> renderer);
            void QueueOperation(shared_ptr<ICanvasOperator> op);
            ~CanvasManager();           

        private:
            //variables
            shared_ptr<CompositeCanvas> canvas;
            shared_ptr<IRenderImages> renderer;
            mutex queueMutex;
            queue<shared_ptr<ICanvasOperator>> operationQueue;
            thread worker_thread;
            atomic<bool> isKilled;
            mutex renderingTimeMutex;
            bool isRendering;
            time_point renderingStartTime;

            //methods
            shared_ptr<ICanvasOperator> DeQueueNextOperation();
            void QueueWorker();
            void RenderingTimeMonitor();
            void KillThreads();

    };

}



