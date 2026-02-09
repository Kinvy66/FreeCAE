/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCSplitter.cpp
 * @brief OCC 分割命令实现（移植自 FITKGeoCompOCC FITKOCCSplitter）
 */
#include "FCOCCSplitter.h"
#include "FCAbstractOCCModel.h"
#include <FCData/FCDataRepo.h>
#include <FCModelInterface/FCModelEnum.h>
#include <BOPAlgo_Splitter.hxx>
#include <BRep_Builder.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Compound.hxx>
#include <TopAbs_ShapeEnum.hxx>
#include <TopExp_Explorer.hxx>
#include <gp_Pnt.hxx>

namespace OCC {

namespace {
TopoDS_Shape getShapeFromSource(const FC::VirtualShape& vs)
{
    if (vs.isNull() || vs.CmdId <= 0) return TopoDS_Shape();
    FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(vs.CmdId);
    if (!cmd) return TopoDS_Shape();
    FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
    if (!occ || !occ->getShape() || occ->getShape()->IsNull()) return TopoDS_Shape();
    return *occ->getShape();
}

TopoDS_Shape getSubShapeFromVirtualShape(const FC::VirtualShape& vs, FC::FCModelEnum::FITKModelSetType type)
{
    if (vs.isNull() || vs.CmdId <= 0) return TopoDS_Shape();
    FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(vs.CmdId);
    if (!cmd) return TopoDS_Shape();
    FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
    if (!occ || !occ->getShape() || occ->getShape()->IsNull()) return TopoDS_Shape();
    if (type == FC::FCModelEnum::FMSSolid)
        return *occ->getShape();
    return occ->getShape(type, vs.VirtualTopoId);
}
} // namespace

FCOCCCurveSplitter::FCOCCCurveSplitter() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FITKGeometryComType FCOCCCurveSplitter::getGeometryCommandType() { return FC::FCGeoEnum::FGTCurveSplitter; }
bool FCOCCCurveSplitter::update()
{
    if (sourceShape().isNull()) return false;
    TopoDS_Shape sourceTopo = getSubShapeFromVirtualShape(sourceShape(), FC::FCModelEnum::FMSEdge);
    if (sourceTopo.IsNull() || sourceTopo.ShapeType() != TopAbs_EDGE) return false;

    TopoDS_Shape toolTopo;
    if (!toolShape().isNull()) {
        toolTopo = getSubShapeFromVirtualShape(toolShape(), FC::FCModelEnum::FMSEdge);
        if (toolTopo.IsNull()) toolTopo = getSubShapeFromVirtualShape(toolShape(), FC::FCModelEnum::FMSSurface);
        if (toolTopo.IsNull()) toolTopo = getShapeFromSource(toolShape());
    } else {
        std::array<double, 3> pt = toolPoint();
        toolTopo = BRepBuilderAPI_MakeVertex(gp_Pnt(pt[0], pt[1], pt[2])).Shape();
    }
    if (toolTopo.IsNull()) return false;

    try {
        BOPAlgo_Splitter splitter;
        splitter.AddArgument(sourceTopo);
        splitter.AddTool(toolTopo);
        splitter.Perform();
        if (splitter.HasErrors()) return false;
        BRep_Builder builder;
        TopoDS_Compound compound;
        builder.MakeCompound(compound);
        for (TopExp_Explorer exp(splitter.Shape(), TopAbs_EDGE); exp.More(); exp.Next())
            builder.Add(compound, exp.Current());
        _occShapeAgent->updateShape(compound);
        return true;
    } catch (...) {
        return false;
    }
}

FCOCCSurfaceSplitter::FCOCCSurfaceSplitter() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FITKGeometryComType FCOCCSurfaceSplitter::getGeometryCommandType() { return FC::FCGeoEnum::FGTSurfaceSplitter; }
bool FCOCCSurfaceSplitter::update()
{
    if (sourceShape().isNull() || toolShape().isNull()) return false;
    TopoDS_Shape sourceTopo = getSubShapeFromVirtualShape(sourceShape(), FC::FCModelEnum::FMSSurface);
    if (sourceTopo.IsNull() || sourceTopo.ShapeType() != TopAbs_FACE) return false;
    TopoDS_Shape toolTopo = getSubShapeFromVirtualShape(toolShape(), FC::FCModelEnum::FMSEdge);
    if (toolTopo.IsNull()) toolTopo = getSubShapeFromVirtualShape(toolShape(), FC::FCModelEnum::FMSSurface);
    if (toolTopo.IsNull()) toolTopo = getShapeFromSource(toolShape());
    if (toolTopo.IsNull()) return false;

    try {
        BOPAlgo_Splitter splitter;
        splitter.AddArgument(sourceTopo);
        splitter.AddTool(toolTopo);
        splitter.Perform();
        if (splitter.HasErrors()) return false;
        BRep_Builder builder;
        TopoDS_Compound compound;
        builder.MakeCompound(compound);
        for (TopExp_Explorer exp(splitter.Shape(), TopAbs_FACE); exp.More(); exp.Next())
            builder.Add(compound, exp.Current());
        _occShapeAgent->updateShape(compound);
        return true;
    } catch (...) {
        return false;
    }
}

FCOCCSolidSplitter::FCOCCSolidSplitter() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FITKGeometryComType FCOCCSolidSplitter::getGeometryCommandType() { return FC::FCGeoEnum::FGTSolidSplitter; }
bool FCOCCSolidSplitter::update()
{
    if (sourceShape().isNull() || toolShape().isNull()) return false;
    TopoDS_Shape sourceTopo = getShapeFromSource(sourceShape());
    if (sourceTopo.IsNull()) return false;
    TopoDS_Shape toolTopo = getShapeFromSource(toolShape());
    if (toolTopo.IsNull()) {
        toolTopo = getSubShapeFromVirtualShape(toolShape(), FC::FCModelEnum::FMSSurface);
        if (toolTopo.IsNull()) toolTopo = getSubShapeFromVirtualShape(toolShape(), FC::FCModelEnum::FMSEdge);
    }
    if (toolTopo.IsNull()) return false;

    try {
        BOPAlgo_Splitter splitter;
        splitter.AddArgument(sourceTopo);
        splitter.AddTool(toolTopo);
        splitter.Perform();
        if (splitter.HasErrors()) return false;
        BRep_Builder builder;
        TopoDS_Compound compound;
        builder.MakeCompound(compound);
        for (TopExp_Explorer exp(splitter.Shape(), TopAbs_SOLID); exp.More(); exp.Next())
            builder.Add(compound, exp.Current());
        _occShapeAgent->updateShape(compound);
        return true;
    } catch (...) {
        return false;
    }
}

} // namespace OCC
