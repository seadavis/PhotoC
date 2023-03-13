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

CanvasManager::CanvasManager(IRenderImages* renderer)
{
    this->renderer = renderer;
    this->canvas = unique_ptr<CompositeCanvas>(new CompositeCanvas());
}

void CanvasManager::QueueOperation(ICanvasOperator& op)
{
    op.Operate(*(canvas.get()));
    auto img = canvas->currentImg();
    this->renderer->RenderImage(img);
}

