#include "canvasmanager.h"


using namespace processing;

void BackgroundImageUpdate::Operate(CompositeCanvas& compositeCanvas)
{
    compositeCanvas.setBackground(backgroundImage);
}

void CompositeImageUpdate::Operate(CompositeCanvas& compositeCanvas)
{
    if(maskPath.length() != 0 && originalImagePath.length() != 0)
    {
        compositeCanvas.setComposite(maskPath, originalImagePath);
    }
}

void TapImage::Operate(CompositeCanvas& compositeCanvas)
{
    compositeCanvas.tap(pointToTap);
}

void TransformImage::Operate(CompositeCanvas& compositeCanvas)
{
    const auto objectType = compositeCanvas.hit(focalPoint);
    OnHit(objectType);

    if(objectType == ObjectType::SizeCircle)
    {
        compositeCanvas.scaleSelected(dx, dy);
    }
    else if(objectType == ObjectType::Image)
    {
        compositeCanvas.translateSelected(dx, dy);
    }
}

void ReleaseImage::Operate(CompositeCanvas& compositeCanvas)
{
    compositeCanvas.releaseObject();
}

void Resize::Operate(CompositeCanvas& compositeCanvas)
{
    compositeCanvas.setSize(width, height);
}

CanvasManager::CanvasManager(CompositeCanvas* canvas, IRenderImages* renderer)
{
    isKilled = false;
    isRendering = false;
    this->renderer = renderer;
    this->canvas = canvas;
    this->worker_thread = thread(&CanvasManager::QueueWorker, this);
}

CanvasManager::~CanvasManager()
{
    this->KillThreads();
}

void CanvasManager::KillThreads()
{
    isKilled = true;
    this->worker_thread.join();
}

void CanvasManager::QueueOperation(shared_ptr<ICanvasOperator> op)
{
    lock_guard<mutex> queueLock(queueMutex);
    operationQueue.push(op);
}

shared_ptr<ICanvasOperator> CanvasManager::DeQueueNextOperation()
{
    lock_guard<mutex> lk(queueMutex);
    shared_ptr<ICanvasOperator> element = nullptr;
    if(!operationQueue.empty())
    {
        element = operationQueue.front();
        operationQueue.pop();
    }

    return element;
}
        
void CanvasManager::QueueWorker()
{
    while(!isKilled)
    {
        try
        {
            auto start = chrono::high_resolution_clock::now();
            shared_ptr<ICanvasOperator> op;

            this->renderer->RenderStarted();
            while((op = DeQueueNextOperation()) != nullptr)
            {
                op->Operate(*canvas);
            }

            auto img = canvas->currentImg();
            if(img.size().area() > 0)
            {
                this->renderer->RenderImage(img);
            }

            this->renderer->RenderStopped();
            auto finish = chrono::high_resolution_clock::now();
            auto milliseconds = chrono::duration_cast<std::chrono::milliseconds>(finish-start);
            auto remaining = chrono::milliseconds(16) - milliseconds;

            if(remaining.count() > 0)
                this_thread::sleep_for(remaining);
        }
        catch(const std::exception& e)
        {
            std::cout << e.what() << '\n';
        }
    }
}


