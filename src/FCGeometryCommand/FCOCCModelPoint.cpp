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
#include "FCAbstractOCCModel.h"
#include <FCGeometryInterface/FCGeoEnum.h>
#include <FCDataRepo.h>
#include <FCModelInterface/FCModelEnum.h>

#include <gp_Pnt.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Face.hxx>
#include <Geom_Curve.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRep_Tool.hxx>
#include <GeomAPI_ProjectPointOnCurve.hxx>
#include <GeomAPI_ProjectPointOnSurf.hxx>
#include <GeomAPI_IntCS.hxx>
#include <BRepExtrema_ExtCC.hxx>
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>
#include <BRep_Builder.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <Precision.hxx>

namespace OCC {

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

static TopoDS_Vertex getVertexFrom(const FC::VirtualShape& vs)
{
    if (vs.CmdId <= 0) return TopoDS_Vertex();
    FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(vs.CmdId);
    if (!cmd) return TopoDS_Vertex();
    FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
    if (!occ || !occ->getShape() || occ->getShape()->IsNull()) return TopoDS_Vertex();
    TopoDS_Shape s = occ->getShape(FC::FCModelEnum::FMSPoint, vs.VirtualTopoId);
    if (s.IsNull() || s.ShapeType() != TopAbs_VERTEX) return TopoDS_Vertex();
    return TopoDS::Vertex(s);
}

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

// ----- CurveEnd -----
FCOCCModelCurveEnd::FCOCCModelCurveEnd() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }

FC::FCGeoEnum::FCGeometryComType FCOCCModelCurveEnd::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTCurveEnd;
}

bool FCOCCModelCurveEnd::update()
{
    if (sourceCurve().isNull()) return false;
    TopoDS_Edge edge = getEdgeFrom(sourceCurve());
    if (edge.IsNull()) return false;
    Standard_Real first, last;
    Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
    if (curve.IsNull()) return false;
    gp_Pnt pnt = useStartEnd() ? curve->Value(first) : curve->Value(last);
    setCoord(pnt.X(), pnt.Y(), pnt.Z());
    _occShapeAgent->updateShape(BRepBuilderAPI_MakeVertex(pnt).Shape());
    return true;
}

// ----- CurveEndDistance -----
FCOCCModelCurveEndDistance::FCOCCModelCurveEndDistance() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }

FC::FCGeoEnum::FCGeometryComType FCOCCModelCurveEndDistance::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTCurveEndDistance;
}

bool FCOCCModelCurveEndDistance::update()
{
    if (sourceCurve().isNull()) return false;
    TopoDS_Edge edge = getEdgeFrom(sourceCurve());
    if (edge.IsNull()) return false;
    Standard_Real first, last;
    Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
    if (curve.IsNull()) return false;
    GProp_GProps props;
    BRepGProp::LinearProperties(edge, props);
    double length = props.Mass();
    if (length < 1e-7 || distance() >= length) return false;
    double t = useStartEnd() ? first + distance() / length * (last - first) : last - distance() / length * (last - first);
    gp_Pnt pnt = curve->Value(t);
    setCoord(pnt.X(), pnt.Y(), pnt.Z());
    _occShapeAgent->updateShape(BRepBuilderAPI_MakeVertex(pnt).Shape());
    return true;
}

// ----- CurveRadio -----
FCOCCModelCurveRadio::FCOCCModelCurveRadio() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }

FC::FCGeoEnum::FCGeometryComType FCOCCModelCurveRadio::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTCurveRadio;
}

bool FCOCCModelCurveRadio::update()
{
    if (sourceCurve().isNull()) return false;
    TopoDS_Edge edge = getEdgeFrom(sourceCurve());
    if (edge.IsNull()) return false;
    Standard_Real first, last;
    Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
    if (curve.IsNull()) return false;
    double t = first + radio() * (last - first);
    gp_Pnt pnt = curve->Value(t);
    setCoord(pnt.X(), pnt.Y(), pnt.Z());
    _occShapeAgent->updateShape(BRepBuilderAPI_MakeVertex(pnt).Shape());
    return true;
}

// ----- PointProjectionOnCurve -----
FCOCCModelPointProjectionOnCurve::FCOCCModelPointProjectionOnCurve() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }

FC::FCGeoEnum::FCGeometryComType FCOCCModelPointProjectionOnCurve::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTPointProjectionOnCurve;
}

bool FCOCCModelPointProjectionOnCurve::update()
{
    if (sourceCurve().isNull() || sourcePoint().isNull()) return false;
    TopoDS_Vertex vertex = getVertexFrom(sourcePoint());
    if (vertex.IsNull()) return false;
    gp_Pnt pnt = BRep_Tool::Pnt(vertex);
    TopoDS_Edge edge = getEdgeFrom(sourceCurve());
    if (edge.IsNull()) return false;
    Standard_Real first, last;
    Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
    if (curve.IsNull()) return false;
    GeomAPI_ProjectPointOnCurve proj(pnt, curve);
    if (proj.NbPoints() == 0) return false;
    QVector<std::array<double, 3>> pts;
    for (int i = 1; i <= proj.NbPoints(); ++i)
    {
        gp_Pnt q = proj.Point(i);
        pts.append({ q.X(), q.Y(), q.Z() });
    }
    setResultPoints(pts);
    setCoord(pts[0][0], pts[0][1], pts[0][2]);
    if (pts.size() == 1)
        _occShapeAgent->updateShape(BRepBuilderAPI_MakeVertex(gp_Pnt(pts[0][0], pts[0][1], pts[0][2])).Shape());
    else
    {
        BRep_Builder builder;
        TopoDS_Compound comp;
        builder.MakeCompound(comp);
        for (const auto& xyz : pts)
            builder.Add(comp, BRepBuilderAPI_MakeVertex(gp_Pnt(xyz[0], xyz[1], xyz[2])).Shape());
        _occShapeAgent->updateShape(comp);
    }
    return true;
}

// ----- TwoCurveIntersectPoint -----
FCOCCModelTwoCurveIntersectPoint::FCOCCModelTwoCurveIntersectPoint() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }

FC::FCGeoEnum::FCGeometryComType FCOCCModelTwoCurveIntersectPoint::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTTwoCurveIntersectPoint;
}

bool FCOCCModelTwoCurveIntersectPoint::update()
{
    if (sourceCurve1().isNull() || sourceCurve2().isNull()) return false;
    TopoDS_Edge edge1 = getEdgeFrom(sourceCurve1());
    TopoDS_Edge edge2 = getEdgeFrom(sourceCurve2());
    if (edge1.IsNull() || edge2.IsNull()) return false;
    BRepExtrema_ExtCC ext(edge1, edge2);
    if (ext.IsParallel()) return false;
    QVector<std::array<double, 3>> pts;
    for (int i = 1; i <= ext.NbExt(); ++i)
    {
        gp_Pnt p1 = ext.PointOnE1(i);
        gp_Pnt p2 = ext.PointOnE2(i);
        if (!isNearestPoint() && p1.Distance(p2) > Precision::Approximation()) continue;
        pts.append({ p1.X(), p1.Y(), p1.Z() });
    }
    if (pts.isEmpty()) return false;
    setResultPoints(pts);
    setCoord(pts[0][0], pts[0][1], pts[0][2]);
    if (pts.size() == 1)
        _occShapeAgent->updateShape(BRepBuilderAPI_MakeVertex(gp_Pnt(pts[0][0], pts[0][1], pts[0][2])).Shape());
    else
    {
        BRep_Builder builder;
        TopoDS_Compound comp;
        builder.MakeCompound(comp);
        for (const auto& xyz : pts)
            builder.Add(comp, BRepBuilderAPI_MakeVertex(gp_Pnt(xyz[0], xyz[1], xyz[2])).Shape());
        _occShapeAgent->updateShape(comp);
    }
    return true;
}

// ----- PointProjectionOnSurface -----
FCOCCModelPointProjectionOnSurface::FCOCCModelPointProjectionOnSurface() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }

FC::FCGeoEnum::FCGeometryComType FCOCCModelPointProjectionOnSurface::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTPointProjectionOnSurface;
}

bool FCOCCModelPointProjectionOnSurface::update()
{
    if (sourcePoint().isNull() || sourceSurface().isNull()) return false;
    TopoDS_Vertex vertex = getVertexFrom(sourcePoint());
    if (vertex.IsNull()) return false;
    gp_Pnt pnt = BRep_Tool::Pnt(vertex);
    TopoDS_Face face = getFaceFrom(sourceSurface());
    if (face.IsNull()) return false;
    Handle(Geom_Surface) surface = BRep_Tool::Surface(face);
    if (surface.IsNull()) return false;
    GeomAPI_ProjectPointOnSurf proj(pnt, surface);
    if (proj.NbPoints() == 0) return false;
    QVector<std::array<double, 3>> pts;
    for (int i = 1; i <= proj.NbPoints(); ++i)
    {
        gp_Pnt q = proj.Point(i);
        pts.append({ q.X(), q.Y(), q.Z() });
    }
    setResultPoints(pts);
    setCoord(pts[0][0], pts[0][1], pts[0][2]);
    if (pts.size() == 1)
        _occShapeAgent->updateShape(BRepBuilderAPI_MakeVertex(gp_Pnt(pts[0][0], pts[0][1], pts[0][2])).Shape());
    else
    {
        BRep_Builder builder;
        TopoDS_Compound comp;
        builder.MakeCompound(comp);
        for (const auto& xyz : pts)
            builder.Add(comp, BRepBuilderAPI_MakeVertex(gp_Pnt(xyz[0], xyz[1], xyz[2])).Shape());
        _occShapeAgent->updateShape(comp);
    }
    return true;
}

// ----- CurveIntersectSurface -----
FCOCCModelCurveIntersectSurface::FCOCCModelCurveIntersectSurface() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }

FC::FCGeoEnum::FCGeometryComType FCOCCModelCurveIntersectSurface::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTCurveIntersectSurface;
}

bool FCOCCModelCurveIntersectSurface::update()
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
    GeomAPI_IntCS ext(trimmed, surface);
    if (ext.NbPoints() <= 0) return false;
    QVector<std::array<double, 3>> pts;
    for (int i = 1; i <= ext.NbPoints(); ++i)
    {
        gp_Pnt q = ext.Point(i);
        pts.append({ q.X(), q.Y(), q.Z() });
    }
    setResultPoints(pts);
    setCoord(pts[0][0], pts[0][1], pts[0][2]);
    if (pts.size() == 1)
        _occShapeAgent->updateShape(BRepBuilderAPI_MakeVertex(gp_Pnt(pts[0][0], pts[0][1], pts[0][2])).Shape());
    else
    {
        BRep_Builder builder;
        TopoDS_Compound comp;
        builder.MakeCompound(comp);
        for (const auto& xyz : pts)
            builder.Add(comp, BRepBuilderAPI_MakeVertex(gp_Pnt(xyz[0], xyz[1], xyz[2])).Shape());
        _occShapeAgent->updateShape(comp);
    }
    return true;
}

} // namespace OCC
