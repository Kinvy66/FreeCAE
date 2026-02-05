#include "FCAbstractGraphObject.h"
#include "FCGraph3DWindowVTK.h"

namespace FC
{
FCAbstractGraphObject::FCAbstractGraphObject(void* dataObject)
    : mDataObject(dataObject)
{
}

FCAbstractGraphObject::~FCAbstractGraphObject() = default;

void FCAbstractGraphObject::setGraphWidget(FCGraph3DWindowVTK* w)
{
    mGraphWidget = w;
}

FCGraph3DWindowVTK* FCAbstractGraphObject::getGraphWidget() const
{
    return mGraphWidget;
}

void FCAbstractGraphObject::removeFromGraphWidget()
{
    if (mGraphWidget)
    {
        mGraphWidget->removeGraphObj(this);
        mGraphWidget = nullptr;
    }
}
} // namespace FC
