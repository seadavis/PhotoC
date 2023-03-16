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

void Resize::Operate(CompositeCanvas& compositeCanvas)
{
    compositeCanvas.setSize(width, height);
}

CanvasManager::CanvasManager(CompositeCanvas* canvas, IRenderImages* renderer)
{
    this->renderer = renderer;
    this->canvas = canvas;
}

void CanvasManager::QueueOperation(shared_ptr<ICanvasOperator> op)
{
    op->Operate(*canvas);
    auto img = canvas->currentImg();
    this->renderer->RenderImage(img);
}

