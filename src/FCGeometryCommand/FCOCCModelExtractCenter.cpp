/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelExtractCenter.cpp
 * @brief 抽圆心/球心命令 OCC 实现（移植自 FITKGeoCompOCC FITKOCCModelExtractCenter）
 */
#include "FCOCCModelExtractCenter.h"
#include "FCAbstractOCCModel.h"
#include <FCDataRepo.h>
#include <FCModelInterface/FCModelEnum.h>

#include <gp_Pnt.hxx>
#include <Geom_SphericalSurface.hxx>
#include <Geom_Circle.hxx>
#include <BRep_Tool.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <GeomAdaptor_Surface.hxx>
#include <GeomAdaptor_Curve.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <TopoDS_Vertex.hxx>

namespace OCC {

FC::FCGeoEnum::FCGeometryComType FCOCCModelExtractCenter::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTExtractCenter;
}

bool FCOCCModelExtractCenter::update()
{
    FC::VirtualShape vs = getShape();
    if (vs.CmdId <= 0) return false;
    FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(vs.CmdId);
    if (!cmd) return false;
    FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
    if (!occ || !occ->getShape() || occ->getShape()->IsNull()) return false;

    gp_Pnt center;
    bool ok = false;
    if (getShapeType() == FC::FCGeoModelExtractCenter::Sphere)
    {
        TopoDS_Shape s = occ->getShape(FC::FCModelEnum::FMSSurface, vs.VirtualTopoId);
        if (s.IsNull() || s.ShapeType() != TopAbs_FACE) return false;
        Handle(Geom_Surface) surface = BRep_Tool::Surface(TopoDS::Face(s));
        GeomAdaptor_Surface adaptor(surface);
        if (adaptor.GetType() == GeomAbs_Sphere)
        {
            Handle(Geom_SphericalSurface) sphere = Handle(Geom_SphericalSurface)::DownCast(surface);
            if (!sphere.IsNull()) { center = sphere->Location(); ok = true; }
        }
    }
    else if (getShapeType() == FC::FCGeoModelExtractCenter::Arc)
    {
        TopoDS_Shape s = occ->getShape(FC::FCModelEnum::FMSEdge, vs.VirtualTopoId);
        if (s.IsNull() || s.ShapeType() != TopAbs_EDGE) return false;
        double start = 0., end = 0.;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(TopoDS::Edge(s), start, end);
        GeomAdaptor_Curve adaptor(curve);
        if (adaptor.GetType() == GeomAbs_Circle)
        {
            Handle(Geom_Circle) circle = Handle(Geom_Circle)::DownCast(curve);
            if (!circle.IsNull()) { center = circle->Location(); ok = true; }
        }
    }
    if (!ok) return false;

    double c[3] = { center.X(), center.Y(), center.Z() };
    setCenter(c);
    TopoDS_Vertex vertex = BRepBuilderAPI_MakeVertex(center);
    _occShapeAgent->updateShape(vertex);
    return true;
}

} // namespace OCC
