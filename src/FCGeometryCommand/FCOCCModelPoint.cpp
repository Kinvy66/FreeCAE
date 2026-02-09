/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelPoint.cpp
 * @brief OCC 点命令实现（移植自 FITKGeoCompOCC FITKOCCModelPoint）
 */
#include "FCOCCModelPoint.h"
#include <gp_Pnt.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>

namespace OCC {

FCOCCModelPoint::FCOCCModelPoint()
    : OCCShapeAgent(this)
{
    _shapeAgent = _occShapeAgent;
}

FC::FCGeoEnum::FCGeometryComType FCOCCModelPoint::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTPoint;
}

bool FCOCCModelPoint::update()
{
    gp_Pnt pt(x(), y(), z());
    TopoDS_Shape shape = BRepBuilderAPI_MakeVertex(pt).Shape();
    _occShapeAgent->updateShape(shape);
    return true;
}

FCOCCModelInteractionPoint::FCOCCModelInteractionPoint()
    : OCCShapeAgent(this)
{
    _shapeAgent = _occShapeAgent;
}

FC::FCGeoEnum::FCGeometryComType FCOCCModelInteractionPoint::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTInteractionPoint;
}

bool FCOCCModelInteractionPoint::update()
{
    return false;
}

} // namespace OCC
