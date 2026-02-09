/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelExportTopos.cpp
 * @brief 导出几何拓扑命令 OCC 实现（移植自 FITKGeoCompOCC FITKOCCModelExportTopos）
 */
#include "FCOCCModelExportTopos.h"
#include "FCAbstractOCCModel.h"
#include <FCGeometryInterface/FCGeoEnum.h>
#include <FCDataRepo.h>
#include <FCModelInterface/FCModelEnum.h>

#include <TopoDS_Shape.hxx>
#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
#include <STEPControl_Writer.hxx>
#include <IGESControl_Writer.hxx>
#include <StlAPI.hxx>

namespace OCC {

static FC::FCModelEnum::FITKModelSetType vTopoToFMS(FC::FCGeoEnum::VTopoShapeType t)
{
    using FT = FC::FCGeoEnum::VTopoShapeType;
    using MT = FC::FCModelEnum::FITKModelSetType;
    switch (t) {
    case FT::VSPoint:  return MT::FMSPoint;
    case FT::VSEdge:   return MT::FMSEdge;
    case FT::VSFace:   return MT::FMSSurface;
    case FT::VSSolid:  return MT::FMSSolid;
    default:           return MT::FMSNone;
    }
}

static bool writeExportFile(const QString& fileName, const TopoDS_Shape& shape)
{
    if (fileName.isEmpty() || shape.IsNull()) return false;
    QString lowerSuffix = fileName.split(".").last().trimmed().toLower();
    QByteArray baName = fileName.toUtf8();
    if (lowerSuffix == "brep")
        return BRepTools::Write(shape, baName);
    if (lowerSuffix == "stp" || lowerSuffix == "step") {
        STEPControl_Writer writer;
        writer.Transfer(shape, STEPControl_AsIs);
        return writer.Write(baName);
    }
    if (lowerSuffix == "igs" || lowerSuffix == "iges") {
        IGESControl_Writer writer;
        writer.AddShape(shape);
        return writer.Write(baName);
    }
    if (lowerSuffix == "stl")
        return StlAPI::Write(shape, baName);
    return false;
}

FC::FCGeoEnum::FITKGeometryComType FCOCCModelExportTopos::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTExportTopos;
}

bool FCOCCModelExportTopos::update()
{
    if (getFileName().isEmpty() || getExportTopos().isEmpty()) return false;

    BRep_Builder builder;
    TopoDS_Compound compound;
    builder.MakeCompound(compound);
    int nValid = 0;
    for (const FC::VirtualShape& topo : getExportTopos())
    {
        FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(topo.CmdId);
        if (!cmd) continue;
        FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
        if (!occ || !occ->getShape() || occ->getShape()->IsNull()) continue;
        FC::FCModelEnum::FITKModelSetType fms = vTopoToFMS(topo.Type);
        if (fms == FC::FCModelEnum::FMSNone) continue;
        TopoDS_Shape s = occ->getShape(fms, topo.VirtualTopoId);
        if (s.IsNull()) continue;
        builder.Add(compound, s);
        nValid++;
    }
    if (nValid == 0) return false;
    return writeExportFile(getFileName(), compound);
}

} // namespace OCC
