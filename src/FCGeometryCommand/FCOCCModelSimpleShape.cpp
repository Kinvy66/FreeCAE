/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelSimpleShape.cpp
 * @brief OCC 基本体实现（移植自 FITKGeoCompOCC FITKOCCModelSimpleShape）
 */
#include "FCOCCModelSimpleShape.h"
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <gp_Ax2.hxx>
#include <gp_Pnt.hxx>

namespace OCC {

FCOCCModelBox::FCOCCModelBox()
    : OCCShapeAgent(this)
{
    _shapeAgent = _occShapeAgent;
}

FC::FCGeoEnum::FCGeometryComType FCOCCModelBox::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTBox;
}

bool FCOCCModelBox::update()
{
    double p1[3], len[3];
    getPoint1(p1);
    getLength(len);
    gp_Pnt pt1(p1[0], p1[1], p1[2]);
    gp_Pnt pt2(p1[0] + len[0], p1[1] + len[1], p1[2] + len[2]);
    if (pt1.Distance(pt2) < 1e-9) return false;
    try {
        BRepPrimAPI_MakeBox cmd(pt1, pt2);
        cmd.Build();
        if (!cmd.IsDone()) return false;
        const TopoDS_Shape& out = cmd.Shape();
        if (out.IsNull()) return false;
        _occShapeAgent->updateShape(out);
        return true;
    } catch (...) {
        return false;
    }
}

FCOCCModelCylinder::FCOCCModelCylinder()
    : OCCShapeAgent(this)
{
    _shapeAgent = _occShapeAgent;
}

FC::FCGeoEnum::FCGeometryComType FCOCCModelCylinder::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTCylinder;
}

bool FCOCCModelCylinder::update()
{
    double loc[3], dir[3];
    getLocation(loc);
    getDirection(dir);
    double r = getRadius();
    double L = getLength();
    if (L < 1e-9 || r < 1e-9) return false;
    try {
        gp_Ax2 axis(gp_Pnt(loc[0], loc[1], loc[2]), gp_Dir(dir[0], dir[1], dir[2]));
        BRepPrimAPI_MakeCylinder cmd(axis, r, L);
        cmd.Build();
        if (!cmd.IsDone()) return false;
        const TopoDS_Shape& out = cmd.Shape();
        if (out.IsNull()) return false;
        _occShapeAgent->updateShape(out);
        return true;
    } catch (...) {
        return false;
    }
}

FCOCCModelSphere::FCOCCModelSphere()
    : OCCShapeAgent(this)
{
    _shapeAgent = _occShapeAgent;
}

FC::FCGeoEnum::FCGeometryComType FCOCCModelSphere::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTSphere;
}

bool FCOCCModelSphere::update()
{
    double loc[3];
    getLocation(loc);
    double r = getRadius();
    if (r < 1e-9) return false;
    try {
        BRepPrimAPI_MakeSphere cmd(gp_Pnt(loc[0], loc[1], loc[2]), r);
        cmd.Build();
        if (!cmd.IsDone()) return false;
        const TopoDS_Shape& out = cmd.Shape();
        if (out.IsNull()) return false;
        _occShapeAgent->updateShape(out);
        return true;
    } catch (...) {
        return false;
    }
}

} // namespace OCC
