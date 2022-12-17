#include "compositeselection.h"
#include <memory>

CompositeSelection::CompositeSelection(shared_ptr<CompositePaths> paths)
{
    layout = new QVBoxLayout(this);
    this->paths = paths;
    sourceSelector = new FileSelector(paths->mask_img_path);
    targetSelector = new FileSelector(paths->original_img_path);
    layout->addWidget(sourceSelector);
    layout->addWidget(targetSelector);
}