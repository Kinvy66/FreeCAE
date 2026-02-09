/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelPartitionSolid.cpp
 * @brief OCC 体分块命令实现（移植自 FITKGeoCompOCC FITKOCCModelPartitionSolid）
 */
#include "FCOCCModelPartitionSolid.h"
#include "FCAbstractOCCModel.h"
#include <FCData/FCDataRepo.h>
#include <FCModelInterface/FCModelEnum.h>
#include <BOPAlgo_Splitter.hxx>
#include <BRep_Builder.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Compound.hxx>
#include <TopAbs_ShapeEnum.hxx>
#include <TopExp_Explorer.hxx>
#include <gp_Pln.hxx>
#include <gp_Dir.hxx>

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
} // namespace

#define FCOCC_PARTITION_SOLID_CTOR(Class) Class::Class() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
#define FCOCC_PARTITION_SOLID_GET_TYPE(Class, Enum) FC::FCGeoEnum::FITKGeometryComType Class::getGeometryCommandType() { return FC::FCGeoEnum::Enum; }
#define FCOCC_PARTITION_SOLID_UPDATE_STUB(Class) bool Class::update() { return false; }

FCOCC_PARTITION_SOLID_CTOR(FCOCCModelPartitionSolidWithPlane)
FCOCC_PARTITION_SOLID_CTOR(FCOCCModelPartitionSolidWithExtendFace)
FCOCC_PARTITION_SOLID_CTOR(FCOCCModelPartitionSolidWithSweepEdge)
FCOCC_PARTITION_SOLID_CTOR(FCOCCModelPartitionSolidWithSketchPlanar)

FCOCC_PARTITION_SOLID_GET_TYPE(FCOCCModelPartitionSolidWithPlane, FGTPartitionSolidWithPlane)
bool FCOCCModelPartitionSolidWithPlane::update()
{
    const QList<FC::VirtualShape>& solidList = solids();
    if (solidList.isEmpty()) return false;
    QVector<double> orig = planeOrigin(), norm = planeNormal();
    if (orig.size() < 3 || norm.size() < 3) return false;
    gp_Pln pln(gp_Pnt(orig[0], orig[1], orig[2]), gp_Dir(norm[0], norm[1], norm[2]));
    TopoDS_Face planeFace = BRepBuilderAPI_MakeFace(pln, -1e6, 1e6, -1e6, 1e6).Face();
    if (planeFace.IsNull()) return false;

    BRep_Builder builder;
    TopoDS_Compound compound;
    builder.MakeCompound(compound);
    try {
        for (const FC::VirtualShape& vs : solidList) {
            TopoDS_Shape solidShape = getShapeFromSource(vs);
            if (solidShape.IsNull()) return false;
            BOPAlgo_Splitter splitter;
            splitter.AddArgument(solidShape);
            splitter.AddTool(planeFace);
            splitter.Perform();
            if (splitter.HasErrors()) return false;
            for (TopExp_Explorer exp(splitter.Shape(), TopAbs_SOLID); exp.More(); exp.Next())
                builder.Add(compound, exp.Current());
        }
        _occShapeAgent->updateShape(compound);
        return true;
    } catch (...) {
        return false;
    }
}

FCOCC_PARTITION_SOLID_GET_TYPE(FCOCCModelPartitionSolidWithExtendFace, FGTPartitionSolidWithExtendFace)
FCOCC_PARTITION_SOLID_UPDATE_STUB(FCOCCModelPartitionSolidWithExtendFace)

FCOCC_PARTITION_SOLID_GET_TYPE(FCOCCModelPartitionSolidWithSweepEdge, FGTPartitionSolidWithSweepEdge)
FCOCC_PARTITION_SOLID_UPDATE_STUB(FCOCCModelPartitionSolidWithSweepEdge)

FCOCC_PARTITION_SOLID_GET_TYPE(FCOCCModelPartitionSolidWithSketchPlanar, FGTPartitionSolidWithSketchPlanar)
FCOCC_PARTITION_SOLID_UPDATE_STUB(FCOCCModelPartitionSolidWithSketchPlanar)

} // namespace OCC
