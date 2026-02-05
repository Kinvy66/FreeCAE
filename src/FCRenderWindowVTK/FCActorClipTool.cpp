/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 */

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
    m_clipType = type;
}

void FCActorClipTool::setClipImplicitFunction(vtkPlane* plane)
{
    m_clipPlane = plane;
}
} // namespace FC
