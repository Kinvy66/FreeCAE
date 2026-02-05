#include "FCGraphOperator.h"
#include "FCGraph3DWindowVTK.h"

namespace FC
{
FCGraphOperator::FCGraphOperator() = default;

FCGraphOperator::~FCGraphOperator() = default;

void FCGraphOperator::setGraph3DWindow(FCGraph3DWindowVTK* w)
{
    mGraph3DWindow = w;
}

FCGraph3DWindowVTK* FCGraphOperator::getGraph3DWindow()
{
    return mGraph3DWindow;
}
} // namespace FC
