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

void Resize::Operate(CompositeCanvas& compositeCanvas)
{
    compositeCanvas.setSize(width, height);
}

CanvasManager::CanvasManager(CompositeCanvas* canvas, IRenderImages* renderer)
{
    this->renderer = renderer;
    this->canvas = canvas;
}

void CanvasManager::QueueOperation(ICanvasOperator& op)
{
    op.Operate(*canvas);
    auto img = canvas->currentImg();
    this->renderer->RenderImage(img);
}

