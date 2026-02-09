/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelNearestPointLine.cpp
 * @brief 最近点线命令 OCC 实现（移植自 FITKGeoCompOCC FITKOCCModelNearestPointLine）
 */
#include "FCOCCModelNearestPointLine.h"
#include "FCAbstractOCCModel.h"
#include <FCDataRepo.h>
#include <FCModelInterface/FCModelEnum.h>

#include <gp_Pnt.hxx>
#include <Geom_Curve.hxx>
#include <Geom_Surface.hxx>
#include <BRep_Tool.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <GeomAPI_ProjectPointOnCurve.hxx>
#include <GeomAPI_ProjectPointOnSurf.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>

namespace OCC {

static bool projectOnCurve(const gp_Pnt& point, const TopoDS_Edge& edge, gp_Pnt& nearest)
{
    double start = 0., end = 0.;
    Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, start, end);
    if (curve.IsNull() || fabs(start - end) < 1e-6) return false;
    GeomAPI_ProjectPointOnCurve projector(point, curve);
    if (projector.NbPoints() < 1) return false;
    nearest = projector.NearestPoint();
    return true;
}

static bool projectOnSurface(const gp_Pnt& point, const TopoDS_Face& face, gp_Pnt& nearest)
{
    Handle(Geom_Surface) surface = BRep_Tool::Surface(face);
    if (surface.IsNull()) return false;
    GeomAPI_ProjectPointOnSurf projector(point, surface);
    if (projector.NbPoints() < 1) return false;
    nearest = projector.NearestPoint();
    return true;
}

FC::FCGeoEnum::FCGeometryComType FCOCCModelNearestPointLine::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTNearestPointLine;
}

bool FCOCCModelNearestPointLine::update()
{
    FC::VirtualShape vs = getShape();
    if (vs.CmdId <= 0) return false;
    FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(vs.CmdId);
    if (!cmd) return false;
    FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
    if (!occ || !occ->getShape() || occ->getShape()->IsNull()) return false;

    gp_Pnt point(m_point[0], m_point[1], m_point[2]);
    gp_Pnt nearest;
    bool ok = false;
    if (getShapeType() == FC::FCGeoModelNearestPointLine::Curve)
    {
        TopoDS_Shape s = occ->getShape(FC::FCModelEnum::FMSEdge, vs.VirtualTopoId);
        if (s.IsNull() || s.ShapeType() != TopAbs_EDGE) return false;
        ok = projectOnCurve(point, TopoDS::Edge(s), nearest);
    }
    else if (getShapeType() == FC::FCGeoModelNearestPointLine::Surface)
    {
        TopoDS_Shape s = occ->getShape(FC::FCModelEnum::FMSSurface, vs.VirtualTopoId);
        if (s.IsNull() || s.ShapeType() != TopAbs_FACE) return false;
        ok = projectOnSurface(point, TopoDS::Face(s), nearest);
    }
    if (!ok) return false;

    m_nearestPoint[0] = nearest.X();
    m_nearestPoint[1] = nearest.Y();
    m_nearestPoint[2] = nearest.Z();

    TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(point, nearest);
    _occShapeAgent->updateShape(edge);
    return true;
}

} // namespace OCC
