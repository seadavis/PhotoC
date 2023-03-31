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
    this->renderer = renderer;
    this->canvas = canvas;
    this->worker_thread = thread(&CanvasManager::QueueWorker, this);
}

void CanvasManager::QueueOperation(shared_ptr<ICanvasOperator> op)
{
    lock_guard<mutex> lk(conditionVariableMutex);
    lock_guard<mutex> queueLock(queueMutex);
    operationQueue.push(op);
    queueCV.notify_one();
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
    while(true)
    {
        try
        {
            unique_lock<mutex> lk(conditionVariableMutex);
            shared_ptr<ICanvasOperator> op;
            while((op = DeQueueNextOperation()) != nullptr)
            {
                op->Operate(*canvas);
            }
            auto img = canvas->currentImg();

            if(img.size().area() > 0)
            {
                this->renderer->RenderImage(img);
                queueCV.wait(lk);
            }

        }
        catch(const std::exception& e)
        {
            std::cout << e.what() << '\n';
        }
    }
}


