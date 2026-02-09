/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelCoverEdges.cpp
 * @brief 覆盖面命令 OCC 实现（移植自 FITKGeoCompOCC FITKOCCModelSurface FITKOCCModelCoverEdges）
 */
#include "FCOCCModelCoverEdges.h"
#include "FCAbstractOCCModel.h"
#include <FCDataRepo.h>
#include <FCGeometryInterface/FCGeoEnum.h>
#include <FCModelInterface/FCModelEnum.h>

#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepFill_Filling.hxx>

#include <QList>

namespace OCC {

static void collectEdges(const FC::VirtualShape& vs, QList<TopoDS_Edge>& out)
{
    if (vs.CmdId <= 0) return;
    FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(vs.CmdId);
    if (!cmd) return;
    FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
    if (!occ || !occ->getShape() || occ->getShape()->IsNull()) return;
    TopoDS_Shape s = occ->getShape(FC::FCModelEnum::FMSEdge, vs.VirtualTopoId);
    if (!s.IsNull() && s.ShapeType() == TopAbs_EDGE)
    {
        out.append(TopoDS::Edge(s));
        return;
    }
    s = *occ->getShape();
    if (!s.IsNull() && s.ShapeType() == TopAbs_COMPOUND)
        for (TopExp_Explorer exp(s, TopAbs_EDGE); exp.More(); exp.Next())
            out.append(TopoDS::Edge(exp.Current()));
}

FC::FCGeoEnum::FITKGeometryComType FCOCCModelCoverEdges::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTCoverEdges;
}

bool FCOCCModelCoverEdges::update()
{
    if (edges().isEmpty()) return false;

    QList<TopoDS_Edge> edgeList;
    for (const FC::VirtualShape& vs : edges())
        collectEdges(vs, edgeList);
    if (edgeList.isEmpty()) return false;

    BRepBuilderAPI_MakeWire wire;
    BRepFill_Filling fillFace;
    for (const TopoDS_Edge& e : edgeList)
    {
        if (e.IsNull()) return false;
        wire.Add(e);
        fillFace.Add(e, GeomAbs_C0, true);
    }
    wire.Build();
    if (!wire.IsDone()) return false;
    TopoDS_Shape wireShape = wire.Shape();
    if (wireShape.IsNull() || wireShape.ShapeType() != TopAbs_WIRE) return false;
    if (!TopoDS::Wire(wireShape).Closed()) return false;

    fillFace.Build();
    if (!fillFace.IsDone()) return false;
    TopoDS_Face face = fillFace.Face();
    if (face.IsNull()) return false;

    _occShapeAgent->updateShape(face);
    return true;
}

} // namespace OCC
