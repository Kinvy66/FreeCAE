/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelOperLine.cpp
 * @brief 线操作命令 OCC 实现（移植自 FITKGeoCompOCC FITKOCCModelOperLine）
 */
#include "FCOCCModelOperLine.h"
#include "FCAbstractOCCModel.h"
#include <FCGeometryInterface/FCGeoEnum.h>
#include <FCDataRepo.h>
#include <FCModelInterface/FCModelEnum.h>

#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <BRep_Tool.hxx>
#include <BRepLib.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>

namespace OCC {

static TopoDS_Shape getEdgeShape(const FC::VirtualShape& vs)
{
    if (vs.CmdId <= 0) return TopoDS_Shape();
    FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(vs.CmdId);
    if (!cmd) return TopoDS_Shape();
    FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
    if (!occ || !occ->getShape() || occ->getShape()->IsNull()) return TopoDS_Shape();
    return occ->getShape(FC::FCModelEnum::FMSEdge, vs.VirtualTopoId);
}

FC::FCGeoEnum::FITKGeometryComType FCOCCModelOperLine::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTMergeLine;
}

bool FCOCCModelOperLine::update()
{
    QList<FC::VirtualShape> vshapes = getVShapes();
    if (vshapes.isEmpty()) return false;

    if (getType() == FC::FCGeoModelOperLine::GBTMerge)
    {
        BRepBuilderAPI_MakeWire mergeWires;
        for (const FC::VirtualShape& vs : vshapes)
        {
            TopoDS_Shape s = getEdgeShape(vs);
            if (s.IsNull() || s.ShapeType() != TopAbs_EDGE) return false;
            mergeWires.Add(TopoDS::Edge(s));
        }
        mergeWires.Build();
        if (!mergeWires.IsDone()) return false;
        _occShapeAgent->updateShape(mergeWires.Shape());
        return true;
    }
    return false;
}

FC::FCGeoEnum::FITKGeometryComType FCOCCModelMiddleOperLine::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTSplitByMiddlePositionLine;
}

bool FCOCCModelMiddleOperLine::update()
{
    QList<FC::VirtualShape> vshapes = getVShapes();
    if (vshapes.size() != 1) return false;

    TopoDS_Shape s = getEdgeShape(vshapes.first());
    if (s.IsNull() || s.ShapeType() != TopAbs_EDGE) return false;
    TopoDS_Edge edge = TopoDS::Edge(s);

    Standard_Real startParam = 0., endParam = 0.;
    Handle(Geom_Curve) c = BRep_Tool::Curve(edge, startParam, endParam);
    if (c.IsNull())
    {
        BRepLib::BuildCurves3d(edge, 1.0e-5, GeomAbs_C1);
        c = BRep_Tool::Curve(edge, startParam, endParam);
    }
    if (c.IsNull()) return false;

    Standard_Real mid = (startParam + endParam) * 0.5;
    Handle(Geom_TrimmedCurve) tc1 = new Geom_TrimmedCurve(c, startParam, mid);
    Handle(Geom_TrimmedCurve) tc2 = new Geom_TrimmedCurve(c, mid, endParam);
    TopoDS_Edge part1 = BRepBuilderAPI_MakeEdge(tc1);
    TopoDS_Edge part2 = BRepBuilderAPI_MakeEdge(tc2);
    BRepBuilderAPI_MakeWire mw(part1, part2);
    if (!mw.IsDone()) return false;
    _occShapeAgent->updateShape(mw.Shape());
    return true;
}

} // namespace OCC
