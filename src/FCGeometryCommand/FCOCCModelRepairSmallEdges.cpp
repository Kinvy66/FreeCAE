/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelRepairSmallEdges.cpp
 * @brief 修复小边命令 OCC 实现（移植自 FITKOCCModelOperLine FITKOCCModelRepairSmallEdges，简化：从形状中移除指定边并用 ShapeFix 修复）
 */
#include "FCOCCModelRepairSmallEdges.h"
#include "FCAbstractOCCModel.h"
#include <FCDataRepo.h>
#include <FCGeometryInterface/FCGeoEnum.h>
#include <FCModelInterface/FCModelEnum.h>

#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <BRep_Builder.hxx>
#include <BRepTools_ReShape.hxx>
#include <ShapeFix_Shape.hxx>

#include <QHash>
#include <QList>

namespace OCC {

static TopoDS_Shape getSubShape(const FC::VirtualShape& vs, FC::FCModelEnum::FITKModelSetType type)
{
    if (vs.CmdId <= 0) return TopoDS_Shape();
    FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(vs.CmdId);
    if (!cmd) return TopoDS_Shape();
    FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
    if (!occ || !occ->getShape() || occ->getShape()->IsNull()) return TopoDS_Shape();
    return occ->getShape(type, vs.VirtualTopoId);
}

FC::FCGeoEnum::FITKGeometryComType FCOCCModelRepairSmallEdges::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTRepairSmallEdges;
}

bool FCOCCModelRepairSmallEdges::update()
{
    QList<FC::VirtualShape> vedges = edges();
    if (vedges.isEmpty()) return false;

    QHash<int, QList<TopoDS_Edge>> cmdIdToEdges;
    QHash<int, TopoDS_Shape> cmdIdToFull;
    for (const FC::VirtualShape& vs : vedges)
    {
        FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(vs.CmdId);
        if (!cmd) return false;
        FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
        if (!occ || !occ->getShape() || occ->getShape()->IsNull()) continue;
        TopoDS_Shape edgeShape = getSubShape(vs, FC::FCModelEnum::FMSEdge);
        if (edgeShape.IsNull() || edgeShape.ShapeType() != TopAbs_EDGE) continue;
        cmdIdToEdges[vs.CmdId].append(TopoDS::Edge(edgeShape));
        if (!cmdIdToFull.contains(vs.CmdId))
            cmdIdToFull[vs.CmdId] = *occ->getShape();
    }
    if (cmdIdToFull.isEmpty()) return false;

    BRep_Builder builder;
    TopoDS_Compound compound;
    builder.MakeCompound(compound);

    for (auto it = cmdIdToFull.begin(); it != cmdIdToFull.end(); ++it)
    {
        int cid = it.key();
        TopoDS_Shape fullShape = it.value();
        const QList<TopoDS_Edge>& edgeList = cmdIdToEdges[cid];
        if (edgeList.isEmpty()) { builder.Add(compound, fullShape); continue; }

        BRepTools_ReShape reShape;
        for (const TopoDS_Edge& e : edgeList)
            reShape.Remove(e);
        TopoDS_Shape result = reShape.Apply(fullShape);
        if (result.IsNull()) result = fullShape;
        else
        {
            ShapeFix_Shape fix(result);
            fix.Perform();
            if (!fix.Shape().IsNull()) result = fix.Shape();
        }
        builder.Add(compound, result);
    }

    _occShapeAgent->updateShape(compound);
    return true;
}

} // namespace OCC
