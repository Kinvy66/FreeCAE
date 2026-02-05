#include "FCActorClipTool.h"
#include <vtkPlane.h>
#include <vtkObjectFactory.h>

namespace FC
{
vtkStandardNewMacro(FCActorClipTool);

FCActorClipTool::FCActorClipTool() = default;

FCActorClipTool::~FCActorClipTool() = default;

void FCActorClipTool::setClipType(ClipType type)
{
    mClipType = type;
}

void FCActorClipTool::setClipImplicitFunction(vtkPlane* plane)
{
    mClipPlane = plane;
}
} // namespace FC
