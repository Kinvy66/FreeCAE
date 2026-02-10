/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelCurve.cpp
 * @brief OCC 曲线命令实现（移植自 FITKGeoCompOCC FITKOCCModelCurve）
 */
#include "FCOCCModelCurve.h"
#include "FCAbstractOCCModel.h"
#include <FCGeometryInterface/FCGeoEnum.h>
#include <FCDataRepo.h>
#include <FCModelInterface/FCModelEnum.h>

#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <gp_Ax2.hxx>
#include <gp_Circ.hxx>
#include <gp_Elips.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <gce_MakeCirc.hxx>
#include <gce_MakeElips.hxx>
#include <gce_MakeHypr.hxx>
#include <gce_MakeParab.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <GC_MakeArcOfEllipse.hxx>
#include <GC_MakeArcOfHyperbola.hxx>
#include <GC_MakeArcOfParabola.hxx>
#include <gp_Hypr.hxx>
#include <gp_Parab.hxx>
#include <gp_Ax1.hxx>
#include <Geom_BezierCurve.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_OffsetCurve.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <GeomAPI_Interpolate.hxx>
#include <GeomProjLib.hxx>
#include <GeomAPI_ProjectPointOnSurf.hxx>
#include <GeomLib_Tool.hxx>
#include <BRep_Tool.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <GeomAPI_ProjectPointOnCurve.hxx>
#include <TopExp_Explorer.hxx>
#include <Precision.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <TColgp_HArray1OfPnt.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <TColStd_Array1OfInteger.hxx>

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

// ----- Line -----
FCOCCModelLine::FCOCCModelLine() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelLine::getGeometryCommandType() { return FC::FCGeoEnum::FGTLine; }
bool FCOCCModelLine::update() { return true; }

// ----- Segment -----
FCOCCModelSegment::FCOCCModelSegment() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelSegment::getGeometryCommandType() { return FC::FCGeoEnum::FGTSegment; }
bool FCOCCModelSegment::update()
{
    std::array<double, 3> s = startPoint();
    std::array<double, 3> e = endPoint();
    gp_Pnt start(s[0], s[1], s[2]);
    gp_Pnt end(e[0], e[1], e[2]);
    try {
        _occShapeAgent->updateShape(BRepBuilderAPI_MakeEdge(start, end).Shape());
    } catch (...) {
        return false;
    }
    return true;
}

// ----- Wire -----
FCOCCModelWire::FCOCCModelWire() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelWire::getGeometryCommandType() { return FC::FCGeoEnum::FGTWire; }
bool FCOCCModelWire::update()
{
    QList<FC::VirtualShape> refs = curves();
    if (refs.size() < 1) return false;
    BRepBuilderAPI_MakeWire wire;
    for (const FC::VirtualShape& vs : refs)
    {
        TopoDS_Edge edge = getEdgeFrom(vs);
        if (edge.IsNull()) return false;
        wire.Add(edge);
    }
    if (!wire.IsDone()) return false;
    _occShapeAgent->updateShape(wire.Shape());
    return true;
}

// ----- Circle -----
FCOCCModelCircle::FCOCCModelCircle() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelCircle::getGeometryCommandType() { return FC::FCGeoEnum::FGTCircle; }
bool FCOCCModelCircle::update()
{
    std::array<double, 3> o = origin();
    std::array<double, 3> z = zDirection();
    std::array<double, 3> y = yDirection();
    gp_Ax2 ax2(gp_Pnt(o[0], o[1], o[2]), gp_Dir(z[0], z[1], z[2]), gp_Dir(y[0], y[1], y[2]));
    gp_Circ c(ax2, radius());
    try {
        _occShapeAgent->updateShape(BRepBuilderAPI_MakeEdge(c).Shape());
    } catch (...) {
        return false;
    }
    return true;
}

// ----- ThreePointsCircle -----
FCOCCModelThreePointsCircle::FCOCCModelThreePointsCircle() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelThreePointsCircle::getGeometryCommandType() { return FC::FCGeoEnum::FGTThreePointsCircle; }
bool FCOCCModelThreePointsCircle::update()
{
    QList<std::array<double, 3>> pts = points();
    if (pts.size() < 3) return false;
    try {
        gce_MakeCirc mkCirc(gp_Pnt(pts[0][0], pts[0][1], pts[0][2]),
                           gp_Pnt(pts[1][0], pts[1][1], pts[1][2]),
                           gp_Pnt(pts[2][0], pts[2][1], pts[2][2]));
        if (!mkCirc.IsDone()) return false;
        gp_Circ c = mkCirc.Value();
        BRepBuilderAPI_MakeEdge mkEdge(c);
        if (!mkEdge.IsDone()) return false;
        setCenter(c.Location().X(), c.Location().Y(), c.Location().Z());
        setRadius(c.Radius());
        _occShapeAgent->updateShape(mkEdge.Shape());
    } catch (...) {
        return false;
    }
    return true;
}

// ----- CircleArc -----
FCOCCModelCircleArc::FCOCCModelCircleArc() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelCircleArc::getGeometryCommandType() { return FC::FCGeoEnum::FGTCircleArc; }
bool FCOCCModelCircleArc::update()
{
    std::array<double, 3> o = origin();
    std::array<double, 3> z = zDirection();
    std::array<double, 3> y = yDirection();
    gp_Ax2 ax2(gp_Pnt(o[0], o[1], o[2]), gp_Dir(z[0], z[1], z[2]), gp_Dir(y[0], y[1], y[2]));
    gp_Circ circ(ax2, radius());
    std::array<double, 3> sp = startPoint();
    std::array<double, 3> ep = endPoint();
    gp_Pnt start(sp[0], sp[1], sp[2]);
    gp_Pnt end(ep[0], ep[1], ep[2]);
    try {
        GC_MakeArcOfCircle arc(circ, start, end, true);
        if (!arc.IsDone()) return false;
        _occShapeAgent->updateShape(BRepBuilderAPI_MakeEdge(arc.Value()).Shape());
    } catch (...) {
        return false;
    }
    return true;
}

// ----- Ellipse -----
FCOCCModelEllipse::FCOCCModelEllipse() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelEllipse::getGeometryCommandType() { return FC::FCGeoEnum::FGTEllipse; }
bool FCOCCModelEllipse::update()
{
    std::array<double, 3> o = origin();
    std::array<double, 3> z = zDirection();
    std::array<double, 3> x = xDirection();
    gp_Ax2 ax2(gp_Pnt(o[0], o[1], o[2]), gp_Dir(z[0], z[1], z[2]), gp_Dir(x[0], x[1], x[2]));
    gp_Elips el(ax2, majorRadius(), minorRadius());
    try {
        _occShapeAgent->updateShape(BRepBuilderAPI_MakeEdge(el).Shape());
    } catch (...) {
        return false;
    }
    return true;
}

// ----- ThreePointsEllipse -----
FCOCCModelThreePointsEllipse::FCOCCModelThreePointsEllipse() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelThreePointsEllipse::getGeometryCommandType() { return FC::FCGeoEnum::FGTThreePointsEllipse; }
bool FCOCCModelThreePointsEllipse::update()
{
    std::array<double, 3> o = origin();
    std::array<double, 3> mp = majorPoint();
    std::array<double, 3> np = minorPoint();
    try {
        gce_MakeElips mkEl(gp_Pnt(mp[0], mp[1], mp[2]), gp_Pnt(np[0], np[1], np[2]), gp_Pnt(o[0], o[1], o[2]));
        if (!mkEl.IsDone()) return false;
        gp_Elips e = mkEl.Value();
        BRepBuilderAPI_MakeEdge mkEdge(e);
        if (!mkEdge.IsDone()) return false;
        setOrigin(e.Location().X(), e.Location().Y(), e.Location().Z());
        _occShapeAgent->updateShape(mkEdge.Shape());
    } catch (...) { return false; }
    return true;
}

// ----- EllipseArc -----
FCOCCModelEllipseArc::FCOCCModelEllipseArc() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelEllipseArc::getGeometryCommandType() { return FC::FCGeoEnum::FGTEllipseArc; }
bool FCOCCModelEllipseArc::update()
{
    std::array<double, 3> o = origin();
    std::array<double, 3> z = zDirection();
    std::array<double, 3> x = xDirection();
    gp_Ax2 ax2(gp_Pnt(o[0], o[1], o[2]), gp_Dir(z[0], z[1], z[2]), gp_Dir(x[0], x[1], x[2]));
    gp_Elips el(ax2, majorRadius(), minorRadius());
    std::array<double, 3> sp = startPoint();
    std::array<double, 3> ep = endPoint();
    try {
        GC_MakeArcOfEllipse arc(el, gp_Pnt(sp[0], sp[1], sp[2]), gp_Pnt(ep[0], ep[1], ep[2]), true);
        if (!arc.IsDone()) return false;
        _occShapeAgent->updateShape(BRepBuilderAPI_MakeEdge(arc.Value()).Shape());
    } catch (...) { return false; }
    return true;
}

// ----- Hyperbola -----
FCOCCModelHyperbola::FCOCCModelHyperbola() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelHyperbola::getGeometryCommandType() { return FC::FCGeoEnum::FGTHyperbola; }
bool FCOCCModelHyperbola::update()
{
    std::array<double, 3> o = origin();
    std::array<double, 3> z = zDirection();
    std::array<double, 3> x = xDirection();
    gp_Ax2 ax2(gp_Pnt(o[0], o[1], o[2]), gp_Dir(z[0], z[1], z[2]), gp_Dir(x[0], x[1], x[2]));
    gp_Hypr h(ax2, majorRadius(), minorRadius());
    try { _occShapeAgent->updateShape(BRepBuilderAPI_MakeEdge(h).Shape()); } catch (...) { return false; }
    return true;
}

// ----- ThreePointsHyperbola -----
FCOCCModelThreePointsHyperbola::FCOCCModelThreePointsHyperbola() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelThreePointsHyperbola::getGeometryCommandType() { return FC::FCGeoEnum::FGTThreePointsHyperbola; }
bool FCOCCModelThreePointsHyperbola::update()
{
    std::array<double, 3> o = origin();
    std::array<double, 3> mp = majorPoint();
    std::array<double, 3> np = minorPoint();
    try {
        gce_MakeHypr mkHy(gp_Pnt(mp[0], mp[1], mp[2]), gp_Pnt(np[0], np[1], np[2]), gp_Pnt(o[0], o[1], o[2]));
        if (!mkHy.IsDone()) return false;
        gp_Hypr h = mkHy.Value();
        BRepBuilderAPI_MakeEdge mkEdge(h);
        if (!mkEdge.IsDone()) return false;
        setOrigin(h.Location().X(), h.Location().Y(), h.Location().Z());
        setMajorRadius(h.MajorRadius());
        setMinorRadius(h.MinorRadius());
        _occShapeAgent->updateShape(mkEdge.Shape());
    } catch (...) { return false; }
    return true;
}

// ----- HyperbolaArc -----
FCOCCModelHyperbolaArc::FCOCCModelHyperbolaArc() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelHyperbolaArc::getGeometryCommandType() { return FC::FCGeoEnum::FGTHyperbolaArc; }
bool FCOCCModelHyperbolaArc::update()
{
    std::array<double, 3> o = origin();
    std::array<double, 3> z = zDirection();
    std::array<double, 3> x = xDirection();
    gp_Ax2 ax2(gp_Pnt(o[0], o[1], o[2]), gp_Dir(z[0], z[1], z[2]), gp_Dir(x[0], x[1], x[2]));
    gp_Hypr h(ax2, majorRadius(), minorRadius());
    std::array<double, 3> sp = startPoint();
    std::array<double, 3> ep = endPoint();
    try {
        GC_MakeArcOfHyperbola arc(h, gp_Pnt(sp[0], sp[1], sp[2]), gp_Pnt(ep[0], ep[1], ep[2]), false);
        if (!arc.IsDone()) return false;
        _occShapeAgent->updateShape(BRepBuilderAPI_MakeEdge(arc.Value()).Shape());
    } catch (...) { return false; }
    return true;
}

// ----- Parabola -----
FCOCCModelParabola::FCOCCModelParabola() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelParabola::getGeometryCommandType() { return FC::FCGeoEnum::FGTParabola; }
bool FCOCCModelParabola::update()
{
    std::array<double, 3> o = origin();
    std::array<double, 3> z = zDirection();
    std::array<double, 3> x = xDirection();
    gp_Ax2 ax2(gp_Pnt(o[0], o[1], o[2]), gp_Dir(z[0], z[1], z[2]), gp_Dir(x[0], x[1], x[2]));
    gp_Parab p(ax2, focalLength());
    try { _occShapeAgent->updateShape(BRepBuilderAPI_MakeEdge(p).Shape()); } catch (...) { return false; }
    return true;
}

// ----- ThreePointsParabola -----
FCOCCModelThreePointsParabola::FCOCCModelThreePointsParabola() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelThreePointsParabola::getGeometryCommandType() { return FC::FCGeoEnum::FGTThreePointsParabola; }
bool FCOCCModelThreePointsParabola::update()
{
    QList<std::array<double, 3>> pts = points();
    if (pts.size() < 3) return false;
    try {
        gp_Ax1 ax1(gp_Pnt(pts[1][0], pts[1][1], pts[1][2]),
                   gp_Dir(pts[2][0] - pts[1][0], pts[2][1] - pts[1][1], pts[2][2] - pts[1][2]));
        gce_MakeParab mkParab(ax1, gp_Pnt(pts[0][0], pts[0][1], pts[0][2]));
        if (!mkParab.IsDone()) return false;
        gp_Parab p = mkParab.Value();
        BRepBuilderAPI_MakeEdge mkEdge(p);
        if (!mkEdge.IsDone()) return false;
        setFocalLength(p.Focal());
        _occShapeAgent->updateShape(mkEdge.Shape());
    } catch (...) { return false; }
    return true;
}

// ----- ParabolaArc -----
FCOCCModelParabolaArc::FCOCCModelParabolaArc() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelParabolaArc::getGeometryCommandType() { return FC::FCGeoEnum::FGTParabolaArc; }
bool FCOCCModelParabolaArc::update()
{
    std::array<double, 3> o = origin();
    std::array<double, 3> z = zDirection();
    std::array<double, 3> x = xDirection();
    gp_Ax2 ax2(gp_Pnt(o[0], o[1], o[2]), gp_Dir(z[0], z[1], z[2]), gp_Dir(x[0], x[1], x[2]));
    gp_Parab p(ax2, focalLength());
    std::array<double, 3> sp = startPoint();
    std::array<double, 3> ep = endPoint();
    try {
        GC_MakeArcOfParabola arc(p, gp_Pnt(sp[0], sp[1], sp[2]), gp_Pnt(ep[0], ep[1], ep[2]), true);
        if (!arc.IsDone()) return false;
        _occShapeAgent->updateShape(BRepBuilderAPI_MakeEdge(arc.Value()).Shape());
    } catch (...) { return false; }
    return true;
}

// ----- BezierByControlPoints -----
FCOCCModelBezierByControlPoints::FCOCCModelBezierByControlPoints() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelBezierByControlPoints::getGeometryCommandType() { return FC::FCGeoEnum::FGTBezierByControlPoints; }
bool FCOCCModelBezierByControlPoints::update()
{
    QList<std::array<double, 3>> pts = controlPoints();
    if (pts.size() < 2) return false;
    TColgp_Array1OfPnt aPoles(1, pts.size());
    for (int i = 0; i < pts.size(); ++i)
        aPoles.SetValue(i + 1, gp_Pnt(pts[i][0], pts[i][1], pts[i][2]));
    try {
        Handle(Geom_BezierCurve) bezier = new Geom_BezierCurve(aPoles);
        BRepBuilderAPI_MakeEdge mkEdge(bezier);
        if (!mkEdge.IsDone()) return false;
        _occShapeAgent->updateShape(mkEdge.Shape());
    } catch (...) { return false; }
    return true;
}

// ----- BsplineByThroughPoints -----
FCOCCModelBsplineByThroughPoints::FCOCCModelBsplineByThroughPoints() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelBsplineByThroughPoints::getGeometryCommandType() { return FC::FCGeoEnum::FGTBSplineByThroughPoints; }
bool FCOCCModelBsplineByThroughPoints::update()
{
    QList<std::array<double, 3>> pts = throughPoints();
    if (!throughPointShapes().isEmpty()) {
        pts.clear();
        for (const FC::VirtualShape& vs : throughPointShapes()) {
            if (vs.CmdId <= 0) return false;
            FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(vs.CmdId);
            if (!cmd) return false;
            FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
            if (!occ || !occ->getShape() || occ->getShape()->IsNull()) return false;
            TopoDS_Shape s = occ->getShape(FC::FCModelEnum::FMSPoint, vs.VirtualTopoId);
            if (s.IsNull() || s.ShapeType() != TopAbs_VERTEX) return false;
            gp_Pnt pnt = BRep_Tool::Pnt(TopoDS::Vertex(s));
            pts.append({ pnt.X(), pnt.Y(), pnt.Z() });
        }
    }
    if (pts.size() < 2) return false;
    Handle(TColgp_HArray1OfPnt) aPoles = new TColgp_HArray1OfPnt(1, pts.size());
    for (int i = 0; i < pts.size(); ++i)
        aPoles->SetValue(i + 1, gp_Pnt(pts[i][0], pts[i][1], pts[i][2]));
    try {
        GeomAPI_Interpolate interp(aPoles, false, 1e-7);
        interp.Perform();
        if (!interp.IsDone()) return false;
        Handle(Geom_BSplineCurve) bspl = interp.Curve();
        BRepBuilderAPI_MakeEdge mkEdge(bspl);
        if (!mkEdge.IsDone()) return false;
        _occShapeAgent->updateShape(mkEdge.Shape());
    } catch (...) { return false; }
    return true;
}

// ----- TrimmedCurve (stub) -----
FCOCCModelTrimmedCurve::FCOCCModelTrimmedCurve() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelTrimmedCurve::getGeometryCommandType() { return FC::FCGeoEnum::FGTTrimmed; }
bool FCOCCModelTrimmedCurve::update() { return false; }

// ----- OffsetCurve -----
FCOCCModelOffsetCurve::FCOCCModelOffsetCurve() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelOffsetCurve::getGeometryCommandType() { return FC::FCGeoEnum::FGTOffsetCurve; }
bool FCOCCModelOffsetCurve::update()
{
    if (sourceCurve().isNull()) return false;
    TopoDS_Edge edge = getEdgeFrom(sourceCurve());
    if (edge.IsNull()) return false;
    Standard_Real first, last;
    Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
    if (curve.IsNull()) return false;
    std::array<double, 3> d = dir();
    gp_Vec vec = curve->DN(first, 1);
    vec.Cross(gp_Vec(d[0], d[1], d[2]));
    try {
        Handle(Geom_OffsetCurve) offsetCurve = new Geom_OffsetCurve(curve, offset(), gp_Dir(-vec.X(), -vec.Y(), -vec.Z()));
        if (offsetCurve.IsNull()) return false;
        BRepBuilderAPI_MakeEdge mkEdge(offsetCurve, first, last);
        mkEdge.Build();
        if (!mkEdge.IsDone()) return false;
        _occShapeAgent->updateShape(mkEdge.Shape());
    } catch (...) { return false; }
    return true;
}

// ----- CurveProjectionOnSurface -----
FCOCCModelCurveProjectionOnSurface::FCOCCModelCurveProjectionOnSurface() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelCurveProjectionOnSurface::getGeometryCommandType() { return FC::FCGeoEnum::FGTCurveProjectionOnSurface; }
bool FCOCCModelCurveProjectionOnSurface::update()
{
    if (sourceCurve().isNull() || sourceSurface().isNull()) return false;
    TopoDS_Edge edge = getEdgeFrom(sourceCurve());
    if (edge.IsNull()) return false;
    Standard_Real first, last;
    Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
    if (curve.IsNull()) return false;
    Handle(Geom_TrimmedCurve) trimmed = new Geom_TrimmedCurve(curve, first, last);
    TopoDS_Face face = getFaceFrom(sourceSurface());
    if (face.IsNull()) return false;
    Handle(Geom_Surface) surface = BRep_Tool::Surface(face);
    if (surface.IsNull()) return false;
    try {
        Handle(Geom_Curve) projCurve = GeomProjLib::Project(trimmed, surface);
        if (projCurve.IsNull()) return false;
        gp_Pnt pntFirst = curve->Value(first);
        gp_Pnt pntLast = curve->Value(last);
        GeomAPI_ProjectPointOnSurf projFirst(pntFirst, surface);
        GeomAPI_ProjectPointOnSurf projLast(pntLast, surface);
        if (projFirst.NbPoints() != 1 || projLast.NbPoints() != 1) return false;
        pntFirst = projFirst.Point(1);
        pntLast = projLast.Point(1);
        Standard_Real newFirst, newLast;
        if (!GeomLib_Tool::Parameter(projCurve, pntFirst, Precision::Confusion(), newFirst) ||
            !GeomLib_Tool::Parameter(projCurve, pntLast, Precision::Confusion(), newLast))
            return false;
        BRepBuilderAPI_MakeEdge mkEdge(projCurve, newFirst, newLast);
        mkEdge.Build();
        if (!mkEdge.IsDone()) return false;
        _occShapeAgent->updateShape(mkEdge.Shape());
    } catch (...) { return false; }
    return true;
}

// ----- SurfaceIntersectionSurface -----
FCOCCModelSurfaceIntersectionSurface::FCOCCModelSurfaceIntersectionSurface() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelSurfaceIntersectionSurface::getGeometryCommandType() { return FC::FCGeoEnum::FGTSurfaceIntersectSurface; }
bool FCOCCModelSurfaceIntersectionSurface::update()
{
    if (sourceSurface1().isNull() || sourceSurface2().isNull()) return false;
    TopoDS_Face face1 = getFaceFrom(sourceSurface1());
    TopoDS_Face face2 = getFaceFrom(sourceSurface2());
    if (face1.IsNull() || face2.IsNull()) return false;
    try {
        BRepAlgoAPI_Section ss(face1, face2);
        if (!ss.IsDone()) return false;
        TopoDS_Shape result = ss.Shape();
        if (result.IsNull()) return false;
        if (result.ShapeType() == TopAbs_EDGE) {
            _occShapeAgent->updateShape(result);
            return true;
        }
        for (TopExp_Explorer exp(result, TopAbs_EDGE); exp.More(); exp.Next()) {
            _occShapeAgent->updateShape(exp.Current());
            return true;
        }
        return false;
    } catch (...) { return false; }
}

// ----- BridgeCurve -----
FCOCCModelBridgeCurve::FCOCCModelBridgeCurve() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelBridgeCurve::getGeometryCommandType() { return FC::FCGeoEnum::FGTBridgeCurve; }
bool FCOCCModelBridgeCurve::update()
{
    if (sourceCurve1().isNull() || sourceCurve2().isNull()) return false;
    double p1 = parameter1() >= 1.0 ? parameter1() : 1.0;
    double p2 = parameter2() >= 1.0 ? parameter2() : 1.0;
    TopoDS_Edge edge1 = getEdgeFrom(sourceCurve1());
    TopoDS_Edge edge2 = getEdgeFrom(sourceCurve2());
    if (edge1.IsNull() || edge2.IsNull()) return false;
    Standard_Real first1, last1;
    Handle(Geom_Curve) curve1 = BRep_Tool::Curve(edge1, first1, last1);
    if (curve1.IsNull()) return false;
    gp_Pnt pnt1 = curve1->Value(first1);
    gp_Vec vec1 = curve1->DN(first1, 1);
    if (vec1.Magnitude() > 1e-12) vec1.Scale(p1 / vec1.Magnitude());
    gp_Pnt pnt2(pnt1.X() - vec1.X(), pnt1.Y() - vec1.Y(), pnt1.Z() - vec1.Z());
    Standard_Real first2, last2;
    Handle(Geom_Curve) curve2 = BRep_Tool::Curve(edge2, first2, last2);
    if (curve2.IsNull()) return false;
    gp_Pnt pnt4 = curve2->Value(first2);
    gp_Vec vec2 = curve2->DN(first2, 1);
    if (vec2.Magnitude() > 1e-12) vec2.Scale(p2 / vec2.Magnitude());
    gp_Pnt pnt3(pnt4.X() - vec2.X(), pnt4.Y() - vec2.Y(), pnt4.Z() - vec2.Z());
    TColgp_Array1OfPnt pnts(1, 4);
    pnts.SetValue(1, pnt1);
    pnts.SetValue(2, pnt2);
    pnts.SetValue(3, pnt3);
    pnts.SetValue(4, pnt4);
    TColStd_Array1OfReal knots(1, 3);
    knots.SetValue(1, 0); knots.SetValue(2, 1); knots.SetValue(3, 2);
    TColStd_Array1OfInteger mults(1, 3);
    mults.SetValue(1, 3); mults.SetValue(2, 1); mults.SetValue(3, 3);
    try {
        Handle(Geom_BSplineCurve) hCurve = new Geom_BSplineCurve(pnts, knots, mults, 2);
        GeomAPI_ProjectPointOnCurve proj1(pnt1, hCurve);
        GeomAPI_ProjectPointOnCurve proj4(pnt4, hCurve);
        if (proj1.NbPoints() < 1 || proj4.NbPoints() < 1) return false;
        Standard_Real par1 = proj1.LowerDistanceParameter();
        Standard_Real par4 = proj4.LowerDistanceParameter();
        BRepBuilderAPI_MakeEdge mkEdge(hCurve, par1, par4);
        _occShapeAgent->updateShape(mkEdge.Shape());
    } catch (...) { return false; }
    return true;
}

// ----- SurfaceEdge -----
FCOCCModelSurfaceEdge::FCOCCModelSurfaceEdge() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelSurfaceEdge::getGeometryCommandType() { return FC::FCGeoEnum::FGTCurveFromSurface; }
bool FCOCCModelSurfaceEdge::update()
{
    if (sourceSurface().isNull() || edgeVirtualTopoId() < 0) return false;
    if (sourceSurface().CmdId <= 0) return false;
    FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(sourceSurface().CmdId);
    if (!cmd) return false;
    FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
    if (!occ || !occ->getShape() || occ->getShape()->IsNull()) return false;
    TopoDS_Shape s = occ->getShape(FC::FCModelEnum::FMSEdge, edgeVirtualTopoId());
    if (s.IsNull() || s.ShapeType() != TopAbs_EDGE) return false;
    TopoDS_Edge edge = TopoDS::Edge(s);
    try {
        Standard_Real first, last;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
        if (curve.IsNull()) return false;
        BRepBuilderAPI_MakeEdge mkEdge(curve, first, last);
        _occShapeAgent->updateShape(mkEdge.Shape());
    } catch (...) { return false; }
    return true;
}

} // namespace OCC
