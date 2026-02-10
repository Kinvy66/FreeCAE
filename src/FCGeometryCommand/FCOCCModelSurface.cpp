/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelSurface.cpp
 * @brief OCC 曲面命令实现（移植自 FITKGeoCompOCC FITKOCCModelSurface）
 */
#include "FCOCCModelSurface.h"
#include "FCAbstractOCCModel.h"
#include <FCGeometryInterface/FCGeoEnum.h>
#include <FCDataRepo.h>
#include <FCModelInterface/FCModelEnum.h>

#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepFill_Filling.hxx>
#include <BRepOffset_MakeOffset.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>
#include <BRepOffsetAPI_MakePipeShell.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>
#include <BRepLib_MakeWire.hxx>
#include <BRep_Tool.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_Copy.hxx>
#include <Geom_Curve.hxx>
#include <Geom_BSplineCurve.hxx>
#include <GeomLib_Tool.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <TColStd_Array1OfInteger.hxx>
#include <Precision.hxx>

namespace OCC {

static TopoDS_Face getFaceFrom(const FC::VirtualShape& vs)
{
    if (vs.CmdId <= 0) return TopoDS_Face();
    FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(vs.CmdId);
    if (!cmd) return TopoDS_Face();
    FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
    if (!occ || !occ->getShape() || occ->getShape()->IsNull()) return TopoDS_Face();
    TopoDS_Shape s = occ->getShape(FC::FCModelEnum::FMSSurface, vs.VirtualTopoId);
    if (s.IsNull() || s.ShapeType() != TopAbs_FACE) return TopoDS_Face();
    return TopoDS::Face(s);
}

static TopoDS_Edge getEdgeFrom(const FC::VirtualShape& vs)
{
    if (vs.CmdId <= 0) return TopoDS_Edge();
    FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(vs.CmdId);
    if (!cmd) return TopoDS_Edge();
    FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
    if (!occ || !occ->getShape() || occ->getShape()->IsNull()) return TopoDS_Edge();
    TopoDS_Shape s = occ->getShape(FC::FCModelEnum::FMSEdge, vs.VirtualTopoId);
    if (s.IsNull() || s.ShapeType() != TopAbs_EDGE) return TopoDS_Edge();
    return TopoDS::Edge(s);
}

/** 从边/线形状得到 Wire：单边则 MakeWire(edge)，多边则依次 Add 后 Build */
static TopoDS_Wire wireFromShape(const FC::VirtualShape& vs)
{
    TopoDS_Edge e = getEdgeFrom(vs);
    if (e.IsNull()) return TopoDS_Wire();
    BRepBuilderAPI_MakeWire mw(e);
    return mw.IsDone() ? TopoDS::Wire(mw.Shape()) : TopoDS_Wire();
}

/** 从多段 VirtualShape（每段一条边）拼成一条 Wire */
static TopoDS_Wire wireFromShapeList(const QList<FC::VirtualShape>& list)
{
    if (list.isEmpty()) return TopoDS_Wire();
    BRepBuilderAPI_MakeWire mw;
    for (const FC::VirtualShape& vs : list)
    {
        TopoDS_Shape s;
        if (vs.CmdId <= 0) return TopoDS_Wire();
        FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(vs.CmdId);
        if (!cmd) return TopoDS_Wire();
        FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
        if (!occ || !occ->getShape() || occ->getShape()->IsNull()) return TopoDS_Wire();
        s = occ->getShape(FC::FCModelEnum::FMSEdge, vs.VirtualTopoId);
        if (s.IsNull()) return TopoDS_Wire();
        if (s.ShapeType() == TopAbs_EDGE)
            mw.Add(TopoDS::Edge(s));
        else if (s.ShapeType() == TopAbs_WIRE)
        {
            for (TopExp_Explorer exp(s, TopAbs_EDGE); exp.More(); exp.Next())
                mw.Add(TopoDS::Edge(exp.Current()));
        }
        else
            return TopoDS_Wire();
    }
    return mw.IsDone() ? TopoDS::Wire(mw.Shape()) : TopoDS_Wire();
}

FCOCCModelClosedWireSurface::FCOCCModelClosedWireSurface() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }

FC::FCGeoEnum::FCGeometryComType FCOCCModelClosedWireSurface::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTClosedWireSurface;
}

bool FCOCCModelClosedWireSurface::update()
{
    QList<FC::VirtualShape> edgeRefs = edges();
    if (edgeRefs.isEmpty()) return false;

    BRepBuilderAPI_MakeWire wire;
    BRepFill_Filling fillFace;
    for (const FC::VirtualShape& vs : edgeRefs)
    {
        if (vs.isNull()) continue;
        TopoDS_Edge edge = getEdgeFrom(vs);
        if (edge.IsNull()) return false;
        fillFace.Add(edge, GeomAbs_C0, true);
        wire.Add(edge);
    }
    if (!wire.IsDone()) return false;
    TopoDS_Shape wireShape = wire.Shape();
    if (wireShape.IsNull() || wireShape.ShapeType() != TopAbs_WIRE) return false;
    if (!TopoDS::Wire(wireShape).Closed()) return false;

    try {
        fillFace.Build();
        if (!fillFace.IsDone()) return false;
        TopoDS_Face face = fillFace.Face();
        if (face.IsNull()) return false;
        _occShapeAgent->updateShape(face);
    } catch (...) {
        return false;
    }
    return true;
}

// ----- OffsetSurface -----
FCOCCModelOffsetSurface::FCOCCModelOffsetSurface() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelOffsetSurface::getGeometryCommandType() { return FC::FCGeoEnum::FGTOffsetSurface; }
bool FCOCCModelOffsetSurface::update()
{
    if (sourceSurface().isNull()) return false;
    TopoDS_Face face = getFaceFrom(sourceSurface());
    if (face.IsNull()) return false;
    try {
        double dist = offset();
        BRepOffset_MakeOffset mkOffset(face, dist, 1e-6);
        if (!mkOffset.IsDone()) return false;
        TopoDS_Shape result = mkOffset.Shape();
        if (result.IsNull()) return false;
        if (result.ShapeType() == TopAbs_FACE) {
            _occShapeAgent->updateShape(result);
            return true;
        }
        for (TopExp_Explorer exp(result, TopAbs_FACE); exp.More(); exp.Next()) {
            _occShapeAgent->updateShape(exp.Current());
            return true;
        }
        return false;
    } catch (...) { return false; }
}

// ----- ExtrudeSurface -----
FCOCCModelExtrudeSurface::FCOCCModelExtrudeSurface() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelExtrudeSurface::getGeometryCommandType() { return FC::FCGeoEnum::FGTExtrudeSurface; }
bool FCOCCModelExtrudeSurface::update()
{
    if (sourceCurve().isNull()) return false;
    TopoDS_Edge edge = getEdgeFrom(sourceCurve());
    if (edge.IsNull()) return false;
    std::array<double, 3> d = direction();
    gp_Vec vec(d[0], d[1], d[2]);
    double mag = vec.Magnitude();
    if (mag <= Precision::Confusion()) return false;
    if (extrudeType() != 1) return false;
    vec.Scale(length() / mag);
    try {
        TopoDS_Shape shape = BRepPrimAPI_MakePrism(edge, vec);
        if (shape.IsNull()) return false;
        if (shape.ShapeType() == TopAbs_FACE) {
            _occShapeAgent->updateShape(shape);
            return true;
        }
        for (TopExp_Explorer exp(shape, TopAbs_FACE); exp.More(); exp.Next()) {
            _occShapeAgent->updateShape(exp.Current());
            return true;
        }
        _occShapeAgent->updateShape(shape);
        return true;
    } catch (...) { return false; }
}

// ----- RevolSurface -----
FCOCCModelRevolSurface::FCOCCModelRevolSurface() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelRevolSurface::getGeometryCommandType() { return FC::FCGeoEnum::FGTRevolSurface; }
bool FCOCCModelRevolSurface::update()
{
    if (sourceCurve().isNull()) return false;
    std::array<double, 3> p1 = rotateAxisPoint1();
    std::array<double, 3> p2 = rotateAxisPoint2();
    gp_Pnt start(p1[0], p1[1], p1[2]);
    gp_Pnt end(p2[0], p2[1], p2[2]);
    if (start.Distance(end) <= Precision::Confusion()) return false;
    gp_Ax1 ax1(start, gp_Dir(end.XYZ() - start.XYZ()));
    TopoDS_Edge edge = getEdgeFrom(sourceCurve());
    if (edge.IsNull()) return false;
    double angleRad = angle() * 3.14159265358979323846 / 180.0;
    try {
        TopoDS_Shape shape = BRepPrimAPI_MakeRevol(edge, ax1, angleRad);
        if (shape.IsNull()) return false;
        if (shape.ShapeType() == TopAbs_FACE) {
            _occShapeAgent->updateShape(shape);
            return true;
        }
        for (TopExp_Explorer exp(shape, TopAbs_FACE); exp.More(); exp.Next()) {
            _occShapeAgent->updateShape(exp.Current());
            return true;
        }
        _occShapeAgent->updateShape(shape);
        return true;
    } catch (...) { return false; }
}

// ----- SweepSurface -----
FCOCCModelSweepSurface::FCOCCModelSweepSurface() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelSweepSurface::getGeometryCommandType() { return FC::FCGeoEnum::FGTSweepSurface; }
bool FCOCCModelSweepSurface::update()
{
    if (profiles().isEmpty() || curves().isEmpty()) return false;
    TopoDS_Wire pathWire = wireFromShapeList(curves());
    if (pathWire.IsNull()) return false;
    TopoDS_Wire profileWire = wireFromShape(profiles().first());
    if (profileWire.IsNull()) return false;
    try
    {
        BRepOffsetAPI_MakePipeShell mp(pathWire);
        mp.SetTransitionMode(BRepBuilderAPI_RightCorner);
        mp.SetDiscreteMode();
        mp.Add(profileWire);
        mp.Build();
        if (!mp.IsDone()) return false;
        TopoDS_Shape result = mp.Shape();
        if (result.IsNull()) return false;
        if (result.ShapeType() == TopAbs_FACE)
        {
            _occShapeAgent->updateShape(result);
            return true;
        }
        for (TopExp_Explorer exp(result, TopAbs_FACE); exp.More(); exp.Next())
        {
            _occShapeAgent->updateShape(exp.Current());
            return true;
        }
        _occShapeAgent->updateShape(result);
        return true;
    }
    catch (...) { return false; }
}

// ----- MultiSectionSurface -----
FCOCCModelMultiSectionSurface::FCOCCModelMultiSectionSurface() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelMultiSectionSurface::getGeometryCommandType() { return FC::FCGeoEnum::FGTMultiSectionSurface; }
bool FCOCCModelMultiSectionSurface::update()
{
    QList<FC::VirtualShape> secs = sections();
    if (secs.size() < 2) return false;
    BRepOffsetAPI_ThruSections thruSection(false);
    for (const FC::VirtualShape& vs : secs)
    {
        TopoDS_Wire w = wireFromShape(vs);
        if (w.IsNull()) return false;
        thruSection.AddWire(w);
    }
    try
    {
        thruSection.Build();
        if (!thruSection.IsDone()) return false;
        TopoDS_Shape result = thruSection.Shape();
        if (result.IsNull()) return false;
        if (result.ShapeType() == TopAbs_FACE)
        {
            _occShapeAgent->updateShape(result);
            return true;
        }
        for (TopExp_Explorer exp(result, TopAbs_FACE); exp.More(); exp.Next())
        {
            _occShapeAgent->updateShape(exp.Current());
            return true;
        }
        _occShapeAgent->updateShape(result);
        return true;
    }
    catch (...) { return false; }
}

// ----- BridgeSurface -----
FCOCCModelBridgeSurface::FCOCCModelBridgeSurface() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelBridgeSurface::getGeometryCommandType() { return FC::FCGeoEnum::FGTBridgeSurface; }
bool FCOCCModelBridgeSurface::update()
{
    if (sourceSurface1().isNull() || sourceSurface2().isNull()) return false;
    if (edgeVirtualTopoId1() < 0 || edgeVirtualTopoId2() < 0) return false;

    FC::FCAbsGeoCommand* cmd1 = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(sourceSurface1().CmdId);
    FC::FCAbsGeoCommand* cmd2 = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(sourceSurface2().CmdId);
    if (!cmd1 || !cmd2) return false;
    FCAbstractOCCModel* occ1 = cmd1->getTShapeAgent<FCAbstractOCCModel>();
    FCAbstractOCCModel* occ2 = cmd2->getTShapeAgent<FCAbstractOCCModel>();
    if (!occ1 || !occ2) return false;

    TopoDS_Shape faceShape1 = occ1->getShape(FC::FCModelEnum::FMSSurface, sourceSurface1().VirtualTopoId);
    TopoDS_Shape edgeShape1 = occ1->getShape(FC::FCModelEnum::FMSEdge, edgeVirtualTopoId1());
    if (faceShape1.IsNull() || faceShape1.ShapeType() != TopAbs_FACE || edgeShape1.IsNull() || edgeShape1.ShapeType() != TopAbs_EDGE)
        return false;
    TopoDS_Face face1 = TopoDS::Face(faceShape1);
    TopoDS_Edge edge1 = TopoDS::Edge(edgeShape1);

    TopoDS_Shape faceShape2 = occ2->getShape(FC::FCModelEnum::FMSSurface, sourceSurface2().VirtualTopoId);
    TopoDS_Shape edgeShape2 = occ2->getShape(FC::FCModelEnum::FMSEdge, edgeVirtualTopoId2());
    if (faceShape2.IsNull() || faceShape2.ShapeType() != TopAbs_FACE || edgeShape2.IsNull() || edgeShape2.ShapeType() != TopAbs_EDGE)
        return false;
    TopoDS_Face face2 = TopoDS::Face(faceShape2);
    TopoDS_Edge edge2 = TopoDS::Edge(edgeShape2);

    Standard_Real first1, last1, first2, last2;
    Handle(Geom_Curve) curve1 = BRep_Tool::Curve(edge1, first1, last1);
    Handle(Geom_Curve) curve2 = BRep_Tool::Curve(edge2, first2, last2);
    if (curve1.IsNull() || curve2.IsNull()) return false;
    gp_Pnt pnt1 = curve1->Value(first1);
    gp_Pnt pnt2 = curve1->Value(last1);
    gp_Pnt pnt3 = curve2->Value(first2);
    gp_Pnt pnt4 = curve2->Value(last2);

    TopoDS_Edge adjacentEdge11, adjacentEdge12, adjacentEdge21, adjacentEdge22;
    for (TopExp_Explorer exp1(face1, TopAbs_EDGE); exp1.More(); exp1.Next())
    {
        TopoDS_Edge edge = TopoDS::Edge(exp1.Current());
        if (edge.IsNull()) continue;
        Standard_Real first, last;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
        gp_Pnt fp = curve->Value(first);
        gp_Pnt lp = curve->Value(last);
        if ((fp.Distance(pnt1) < Precision::Confusion() && lp.Distance(pnt2) < Precision::Confusion())
            || (fp.Distance(pnt2) < Precision::Confusion() && lp.Distance(pnt1) < Precision::Confusion()))
            continue;
        if (fp.Distance(pnt1) < Precision::Confusion() || lp.Distance(pnt1) < Precision::Confusion())
            adjacentEdge11 = edge;
        else if (fp.Distance(pnt2) < Precision::Confusion() || lp.Distance(pnt2) < Precision::Confusion())
            adjacentEdge12 = edge;
    }
    for (TopExp_Explorer exp2(face2, TopAbs_EDGE); exp2.More(); exp2.Next())
    {
        TopoDS_Edge edge = TopoDS::Edge(exp2.Current());
        if (edge.IsNull()) continue;
        Standard_Real first, last;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
        gp_Pnt fp = curve->Value(first);
        gp_Pnt lp = curve->Value(last);
        if ((fp.Distance(pnt3) < Precision::Confusion() && lp.Distance(pnt4) < Precision::Confusion())
            || (fp.Distance(pnt4) < Precision::Confusion() && lp.Distance(pnt3) < Precision::Confusion()))
            continue;
        if (fp.Distance(pnt3) < Precision::Confusion() || lp.Distance(pnt3) < Precision::Confusion())
            adjacentEdge21 = edge;
        else if (fp.Distance(pnt4) < Precision::Confusion() || lp.Distance(pnt4) < Precision::Confusion())
            adjacentEdge22 = edge;
    }
    if (adjacentEdge11.IsNull() || adjacentEdge12.IsNull() || adjacentEdge21.IsNull() || adjacentEdge22.IsNull())
        return false;

    Standard_Real first, last;
    GeomLib_Tool parameterTool;

    auto makeSplineEdge = [&parameterTool](const gp_Pnt& pA, const gp_Pnt& pB,
        const TopoDS_Edge& adjA, const TopoDS_Edge& adjB) -> TopoDS_Edge
    {
        Standard_Real fa, la, fb, lb;
        Handle(Geom_Curve) ca = BRep_Tool::Curve(adjA, fa, la);
        Handle(Geom_Curve) cb = BRep_Tool::Curve(adjB, fb, lb);
        bool pAIsStart = ca->Value(fa).Distance(pA) < Precision::Confusion();
        gp_Vec va = pAIsStart ? ca->DN(fa, 1) : ca->DN(la, 1);
        gp_Pnt adjPntA = pAIsStart ? gp_Pnt(pA.X() - va.X(), pA.Y() - va.Y(), pA.Z() - va.Z())
                                   : gp_Pnt(pA.X() + va.X(), pA.Y() + va.Y(), pA.Z() + va.Z());
        bool pBIsStart = cb->Value(fb).Distance(pB) < Precision::Confusion();
        gp_Vec vb = pBIsStart ? cb->DN(fb, 1) : cb->DN(lb, 1);
        gp_Pnt adjPntB = pBIsStart ? gp_Pnt(pB.X() - vb.X(), pB.Y() - vb.Y(), pB.Z() - vb.Z())
                                   : gp_Pnt(pB.X() + vb.X(), pB.Y() + vb.Y(), pB.Z() + vb.Z());
        TColgp_Array1OfPnt pnts(1, 4);
        pnts.SetValue(1, pA);
        pnts.SetValue(2, adjPntA);
        pnts.SetValue(3, adjPntB);
        pnts.SetValue(4, pB);
        TColStd_Array1OfReal knots(1, 3);
        knots.SetValue(1, 0); knots.SetValue(2, 1); knots.SetValue(3, 2);
        TColStd_Array1OfInteger mults(1, 3);
        mults.SetValue(1, 3); mults.SetValue(2, 1); mults.SetValue(3, 3);
        Handle(Geom_BSplineCurve) hCurve = new Geom_BSplineCurve(pnts, knots, mults, 2);
        Standard_Real prA, prB;
        parameterTool.Parameter(hCurve, pA, Precision::Confusion(), prA);
        parameterTool.Parameter(hCurve, pB, Precision::Confusion(), prB);
        BRepBuilderAPI_MakeEdge me(hCurve, prA, prB);
        return me.IsDone() ? me.Edge() : TopoDS_Edge();
    };

    TopoDS_Edge anEdge13 = makeSplineEdge(pnt1, pnt3, adjacentEdge11, adjacentEdge21);
    TopoDS_Edge anEdge24 = makeSplineEdge(pnt2, pnt4, adjacentEdge12, adjacentEdge22);
    if (anEdge13.IsNull() || anEdge24.IsNull()) return false;

    BRepFill_Filling fillFace;
    fillFace.Add(edge1, GeomAbs_C0, true);
    fillFace.Add(anEdge13, GeomAbs_C0, true);
    fillFace.Add(edge2, GeomAbs_C0, true);
    fillFace.Add(anEdge24, GeomAbs_C0, true);
    try
    {
        fillFace.Build();
        if (!fillFace.IsDone()) return false;
        _occShapeAgent->updateShape(fillFace.Face());
        return true;
    }
    catch (...) { return false; }
}

// ----- SolidSurface -----
FCOCCModelSolidSurface::FCOCCModelSolidSurface() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelSolidSurface::getGeometryCommandType() { return FC::FCGeoEnum::FGTSurfaceFromSolid; }
bool FCOCCModelSolidSurface::update()
{
    if (solid().isNull() || faceVirtualTopoId() < 0) return false;
    if (solid().CmdId <= 0) return false;
    FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(solid().CmdId);
    if (!cmd) return false;
    FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
    if (!occ || !occ->getShape() || occ->getShape()->IsNull()) return false;
    TopoDS_Shape s = occ->getShape(FC::FCModelEnum::FMSSurface, faceVirtualTopoId());
    if (s.IsNull() || s.ShapeType() != TopAbs_FACE) return false;
    _occShapeAgent->updateShape(s);
    return true;
}

} // namespace OCC
