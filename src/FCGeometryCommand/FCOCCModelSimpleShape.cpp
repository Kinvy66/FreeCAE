/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelSimpleShape.cpp
 * @brief OCC 基本体实现（盒、圆柱、球、圆锥、圆环、螺旋）
 */
#include "FCOCCModelSimpleShape.h"
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeTorus.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepOffsetAPI_MakePipe.hxx>
#include <gp_Ax2.hxx>
#include <gp_Pnt.hxx>
#include <gp_Circ.hxx>
#include <BRepLib.hxx>
#include <cmath>

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

FCOCCModelCone::FCOCCModelCone()
    : OCCShapeAgent(this)
{
    _shapeAgent = _occShapeAgent;
}

FC::FCGeoEnum::FCGeometryComType FCOCCModelCone::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTCone;
}

bool FCOCCModelCone::update()
{
    double loc[3], axis[3];
    getLocation(loc);
    getAxis(axis);
    double h = getHeight();
    double r1 = getBottomRadius();
    double r2 = getTopRadius();
    if (h < 1e-9) return false;
    try {
        gp_Ax2 ax(gp_Pnt(loc[0], loc[1], loc[2]), gp_Dir(axis[0], axis[1], axis[2]));
        BRepPrimAPI_MakeCone cmd(ax, r1, r2, h);
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

FCOCCModelTorus::FCOCCModelTorus()
    : OCCShapeAgent(this)
{
    _shapeAgent = _occShapeAgent;
}

FC::FCGeoEnum::FCGeometryComType FCOCCModelTorus::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTTorus;
}

bool FCOCCModelTorus::update()
{
    double loc[3], axis[3];
    getLocation(loc);
    getAxis(axis);
    double r1 = getMajorRadius();
    double r2 = getMinorRadius();
    double angle = getAngle();
    if (r1 < 1e-9 || r2 < 1e-9 || r2 > r1) return false;
    try {
        gp_Ax2 ax(gp_Pnt(loc[0], loc[1], loc[2]), gp_Dir(axis[0], axis[1], axis[2]));
        BRepPrimAPI_MakeTorus cmd(ax, r1, r2);
        if (angle < 360.0 - 1e-6) {
            const double angRad = angle * M_PI / 180.0;
            cmd = BRepPrimAPI_MakeTorus(ax, r1, r2, angRad);
        }
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

FCOCCModelHelix::FCOCCModelHelix()
    : OCCShapeAgent(this)
{
    _shapeAgent = _occShapeAgent;
}

FC::FCGeoEnum::FCGeometryComType FCOCCModelHelix::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTHelix;
}

bool FCOCCModelHelix::update()
{
    double loc[3];
    getLocation(loc);
    int nTurns = getNumberOfTurns();
    double majorR = getMajorRadius();
    double minorR = getMinorRadius();
    double axialPitch = getAxialPitch();
    int chirality = getChirality();
    if (nTurns < 1 || majorR < 1e-9 || minorR < 1e-9 || minorR >= majorR) return false;
    if (axialPitch < 1e-9) axialPitch = majorR * 0.5;
    const int chir = (chirality >= 0) ? 1 : -1;

    try {
        const int nSeg = (nTurns * 24 > 16) ? (nTurns * 24) : 16;
        const double dTheta = 2.0 * M_PI * nTurns / nSeg;
        BRepBuilderAPI_MakeWire mkWire;
        gp_Pnt pPrev(loc[0] + majorR, loc[1], loc[2]);
        for (int i = 1; i <= nSeg; ++i) {
            double th = chir * i * dTheta;
            gp_Pnt pCur(loc[0] + majorR * std::cos(th),
                        loc[1] + majorR * std::sin(th),
                        loc[2] + axialPitch * i * dTheta / (2.0 * M_PI));
            TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(pPrev, pCur);
            mkWire.Add(edge);
            pPrev = pCur;
        }
        if (!mkWire.IsDone()) return false;

        gp_Pnt p0(loc[0] + majorR, loc[1], loc[2]);
        gp_Pnt p1(loc[0] + majorR * std::cos(chir * dTheta),
                  loc[1] + majorR * std::sin(chir * dTheta),
                  loc[2] + axialPitch * dTheta / (2.0 * M_PI));
        gp_Dir tangent(p1.X() - p0.X(), p1.Y() - p0.Y(), p1.Z() - p0.Z());
        gp_Ax2 circAx(p0, tangent);
        gp_Circ circle(circAx, minorR);
        TopoDS_Edge circleEdge = BRepBuilderAPI_MakeEdge(circle);
        BRepBuilderAPI_MakeWire circleWire(circleEdge);
        if (!circleWire.IsDone()) return false;

        BRepOffsetAPI_MakePipe pipe(mkWire.Wire(), circleWire.Wire());
        pipe.Build();
        if (!pipe.IsDone()) return false;
        const TopoDS_Shape& out = pipe.Shape();
        if (out.IsNull()) return false;
        _occShapeAgent->updateShape(out);
        return true;
    } catch (...) {
        return false;
    }
}

} // namespace OCC
