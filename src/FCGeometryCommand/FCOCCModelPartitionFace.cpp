/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelPartitionFace.cpp
 * @brief OCC 面分块命令实现（移植自 FITKGeoCompOCC FITKOCCModelPartitionFace）
 */
#include "FCOCCModelPartitionFace.h"
#include "FCAbstractOCCModel.h"
#include <FCData/FCDataRepo.h>
#include <FCGeometryInterface/FCAbsGeoDatum.h>
#include <FCModelInterface/FCModelEnum.h>
#include <BOPAlgo_Splitter.hxx>
#include <BRep_Builder.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Compound.hxx>
#include <TopAbs_ShapeEnum.hxx>
#include <TopExp_Explorer.hxx>
#include <gp_Pln.hxx>
#include <gp_Dir.hxx>

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

#define FCOCC_PARTITION_FACE_CTOR(Class) Class::Class() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
#define FCOCC_PARTITION_FACE_GET_TYPE(Class, Enum) FC::FCGeoEnum::FITKGeometryComType Class::getGeometryCommandType() { return FC::FCGeoEnum::Enum; }
#define FCOCC_PARTITION_FACE_UPDATE_STUB(Class) bool Class::update() { return false; }

FCOCC_PARTITION_FACE_CTOR(FCOCCModelPartitionFaceWithSketch)
FCOCC_PARTITION_FACE_CTOR(FCOCCModelPartitionFaceWithTwoPoints)
FCOCC_PARTITION_FACE_CTOR(FCOCCModelPartitionFaceWithDatumPlane)
FCOCC_PARTITION_FACE_CTOR(FCOCCModelPartitionFaceWithCurvedPath)
FCOCC_PARTITION_FACE_CTOR(FCOCCModelPartitionFaceWithExtendFace)
FCOCC_PARTITION_FACE_CTOR(FCOCCModelPartitionFaceWithIntersectFace)
FCOCC_PARTITION_FACE_CTOR(FCOCCModelPartitionFaceWithProjectEdges)

FCOCC_PARTITION_FACE_GET_TYPE(FCOCCModelPartitionFaceWithSketch, FGTPartitionFaceWithSketch)
FCOCC_PARTITION_FACE_UPDATE_STUB(FCOCCModelPartitionFaceWithSketch)

FCOCC_PARTITION_FACE_GET_TYPE(FCOCCModelPartitionFaceWithTwoPoints, FGTPartitionFaceWithTwoPoints)
FCOCC_PARTITION_FACE_UPDATE_STUB(FCOCCModelPartitionFaceWithTwoPoints)

FCOCC_PARTITION_FACE_GET_TYPE(FCOCCModelPartitionFaceWithDatumPlane, FGTPartitionFaceWithDatumPlane)
bool FCOCCModelPartitionFaceWithDatumPlane::update()
{
    FC::FCAbsGeoDatumPlane* datum = datumPlane();
    if (!datum || faces().isEmpty()) return false;
    double pos[3], nor[3];
    datum->getPosition(pos);
    datum->getNormal(nor);
    gp_Pln pln(gp_Pnt(pos[0], pos[1], pos[2]), gp_Dir(nor[0], nor[1], nor[2]));
    TopoDS_Face planeFace = BRepBuilderAPI_MakeFace(pln, -1e6, 1e6, -1e6, 1e6).Face();
    if (planeFace.IsNull()) return false;

    BRep_Builder builder;
    TopoDS_Compound compound;
    builder.MakeCompound(compound);
    try {
        for (const FC::VirtualShape& vs : faces()) {
            TopoDS_Shape fs = getSubShapeFromVirtualShape(vs, FC::FCModelEnum::FMSSurface);
            if (fs.IsNull() || fs.ShapeType() != TopAbs_FACE) return false;
            BOPAlgo_Splitter splitter;
            splitter.AddArgument(fs);
            splitter.AddTool(planeFace);
            splitter.Perform();
            if (splitter.HasErrors()) return false;
            for (TopExp_Explorer exp(splitter.Shape(), TopAbs_FACE); exp.More(); exp.Next())
                builder.Add(compound, exp.Current());
        }
        _occShapeAgent->updateShape(compound);
        return true;
    } catch (...) {
        return false;
    }
}

FCOCC_PARTITION_FACE_GET_TYPE(FCOCCModelPartitionFaceWithCurvedPath, FGTPartitionFaceWithCurvedPath)
FCOCC_PARTITION_FACE_UPDATE_STUB(FCOCCModelPartitionFaceWithCurvedPath)

FCOCC_PARTITION_FACE_GET_TYPE(FCOCCModelPartitionFaceWithExtendFace, FGTPartitionFaceWithExtendFace)
FCOCC_PARTITION_FACE_UPDATE_STUB(FCOCCModelPartitionFaceWithExtendFace)

FCOCC_PARTITION_FACE_GET_TYPE(FCOCCModelPartitionFaceWithIntersectFace, FGTPartitionFaceWithIntersectFace)
FCOCC_PARTITION_FACE_UPDATE_STUB(FCOCCModelPartitionFaceWithIntersectFace)

FCOCC_PARTITION_FACE_GET_TYPE(FCOCCModelPartitionFaceWithProjectEdges, FGTPartitionFaceWithProjectEdges)
FCOCC_PARTITION_FACE_UPDATE_STUB(FCOCCModelPartitionFaceWithProjectEdges)

} // namespace OCC
