/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelStitch.cpp
 * @brief 缝合面命令 OCC 实现（移植自 FITKGeoCompOCC FITKOCCModelStitch）
 */
#include "FCOCCModelStitch.h"
#include "FCAbstractOCCModel.h"
#include <FCGeometryInterface/FCGeoEnum.h>
#include <FCDataRepo.h>
#include <FCModelInterface/FCModelEnum.h>

#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Shell.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Compound.hxx>
#include <TopExp_Explorer.hxx>
#include <BRep_Builder.hxx>
#include <BRepBuilderAPI_Sewing.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <BRepTools_ReShape.hxx>

namespace OCC {

static TopoDS_Shape getShapeFromSource(const FC::VirtualShape& vs)
{
    if (vs.CmdId <= 0) return TopoDS_Shape();
    FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(vs.CmdId);
    if (!cmd) return TopoDS_Shape();
    FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
    if (!occ || !occ->getShape() || occ->getShape()->IsNull()) return TopoDS_Shape();
    return *occ->getShape();
}

static TopoDS_Shape getEdgeShape(const FC::VirtualShape& vs)
{
    if (vs.CmdId <= 0) return TopoDS_Shape();
    FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(vs.CmdId);
    if (!cmd) return TopoDS_Shape();
    FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
    if (!occ || !occ->getShape() || occ->getShape()->IsNull()) return TopoDS_Shape();
    return occ->getShape(FC::FCModelEnum::FMSEdge, vs.VirtualTopoId);
}

FC::FCGeoEnum::FCGeometryComType FCOCCModelStitch::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTStitch;
}

bool FCOCCModelStitch::update()
{
    TopoDS_Shape baseShape = getShapeFromSource(getInputCmdId());
    if (baseShape.IsNull()) return false;

    BRepBuilderAPI_Sewing sewing;
    sewing.Load(baseShape);
    sewing.SetTolerance(tolerance());

    for (const FC::VirtualShape& edge : edges())
    {
        TopoDS_Shape edgeShape = getEdgeShape(edge);
        if (edgeShape.IsNull()) return false;
        sewing.Add(edgeShape);
    }

    TopoDS_Shape resultShape;
    try
    {
        sewing.Perform();
        resultShape = sewing.SewedShape();

        BRep_Builder builder;
        TopoDS_Compound compound;
        builder.MakeCompound(compound);
        BRepTools_ReShape reShape;
        for (TopExp_Explorer iter(resultShape, TopAbs_ShapeEnum::TopAbs_SHELL); iter.More(); iter.Next())
        {
            const TopoDS_Shell& ss = TopoDS::Shell(iter.Value());
            TopoDS_Solid solid = BRepBuilderAPI_MakeSolid(ss);
            if (!solid.IsNull())
            {
                reShape.Remove(ss);
                builder.Add(compound, solid);
            }
        }
        TopoDS_Shape newShape = reShape.Apply(resultShape);
        if (!newShape.IsNull())
            builder.Add(compound, newShape);
        resultShape = compound;
    }
    catch (...)
    {
        return false;
    }

    _occShapeAgent->updateShape(resultShape);
    return true;
}

} // namespace OCC
