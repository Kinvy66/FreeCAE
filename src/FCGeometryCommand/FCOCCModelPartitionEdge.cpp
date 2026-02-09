/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelPartitionEdge.cpp
 * @brief OCC 边分块命令实现（移植自 FITKGeoCompOCC FITKOCCModelPartitionEdge）
 */
#include "FCOCCModelPartitionEdge.h"
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
#include <BRep_Tool.hxx>
#include <Geom_Curve.hxx>
#include <gp_Pnt.hxx>

namespace OCC {

namespace {
TopoDS_Shape getSubShapeFromVirtualShape(const FC::VirtualShape& vs, FC::FCModelEnum::FITKModelSetType type)
{
    if (vs.isNull() || vs.CmdId <= 0) return TopoDS_Shape();
    FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(vs.CmdId);
    if (!cmd) return TopoDS_Shape();
    FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
    if (!occ || !occ->getShape() || occ->getShape()->IsNull()) return TopoDS_Shape();
    return occ->getShape(type, vs.VirtualTopoId);
}
} // namespace

FCOCCModelPartitionEdgeWithParameter::FCOCCModelPartitionEdgeWithParameter() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelPartitionEdgeWithParameter::getGeometryCommandType() { return FC::FCGeoEnum::FGTPartitionEdgeWithParameter; }
bool FCOCCModelPartitionEdgeWithParameter::update()
{
    const QList<FC::VirtualShape>& edgeList = edges();
    if (edgeList.isEmpty()) return false;
    double param = parameter();
    if (param <= 0.0 || param >= 1.0) return false;

    BRep_Builder builder;
    TopoDS_Compound compound;
    builder.MakeCompound(compound);
    try {
        for (const FC::VirtualShape& vs : edgeList) {
            TopoDS_Shape es = getSubShapeFromVirtualShape(vs, FC::FCModelEnum::FMSEdge);
            if (es.IsNull() || es.ShapeType() != TopAbs_EDGE) return false;
            TopoDS_Edge edge = TopoDS::Edge(es);
            Standard_Real first, last;
            Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
            if (curve.IsNull()) return false;
            double t = first + (last - first) * param;
            gp_Pnt pt = curve->Value(t);
            TopoDS_Shape vertex = BRepBuilderAPI_MakeVertex(pt).Shape();
            BOPAlgo_Splitter splitter;
            splitter.AddArgument(edge);
            splitter.AddTool(vertex);
            splitter.Perform();
            if (splitter.HasErrors()) return false;
            for (TopExp_Explorer exp(splitter.Shape(), TopAbs_EDGE); exp.More(); exp.Next())
                builder.Add(compound, exp.Current());
        }
        _occShapeAgent->updateShape(compound);
        return true;
    } catch (...) {
        return false;
    }
}

FCOCCModelPartitionEdgeWithPoint::FCOCCModelPartitionEdgeWithPoint() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelPartitionEdgeWithPoint::getGeometryCommandType() { return FC::FCGeoEnum::FGTPartitionEdgeWithPoint; }
bool FCOCCModelPartitionEdgeWithPoint::update() { return false; }

FCOCCModelPartitionEdgeWithDatumPlane::FCOCCModelPartitionEdgeWithDatumPlane() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelPartitionEdgeWithDatumPlane::getGeometryCommandType() { return FC::FCGeoEnum::FGTPartitionEdgeWithDatumPlane; }
bool FCOCCModelPartitionEdgeWithDatumPlane::update() { return false; }

} // namespace OCC
