#include "canvasmanager.h"

using namespace processing;

Mat BackgroundImageUpdate::Operate(CompositeCanvas& compositeCanvas)
{
    return Mat(0,3,CV_32FC1);
}

Mat CompositeImageUpdate::Operate(CompositeCanvas& compositeCanvas)
{
     return Mat(0,3,CV_32FC1);
}


CanvasManager::CanvasManager(IRenderImages* renderer)
{
    this->renderer = renderer;
    this->canvas = unique_ptr<CompositeCanvas>(new CompositeCanvas());
}

void CanvasManager::QueueOperation(ICanvasOperator& op)
{

}

