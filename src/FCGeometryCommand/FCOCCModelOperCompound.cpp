/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelOperCompound.cpp
 * @brief 复合体操作命令 OCC 实现（移植自 FITKGeoCompOCC FITKOCCModelOperCompound）
 */
#include "FCOCCModelOperCompound.h"
#include "FCAbstractOCCModel.h"
#include <FCDataRepo.h>

#include <TopoDS_Shape.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_Iterator.hxx>
#include <BRep_Builder.hxx>

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

FC::FCGeoEnum::FCGeometryComType FCOCCModelOperAddCompound::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTCompoundAppendShape;
}

bool FCOCCModelOperAddCompound::update()
{
    const QList<FC::VirtualShape>& ids = getShapeCmdIds();
    const FC::VirtualShape& baseVs = getInputCmdId();
    if (ids.isEmpty() || baseVs.CmdId <= 0) return false;
    if (ids.first().CmdId == baseVs.CmdId) return false;

    FC::FCAbsGeoCommand* baseCmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(baseVs.CmdId);
    if (!baseCmd) return false;
    FCAbstractOCCModel* baseOcc = baseCmd->getTShapeAgent<FCAbstractOCCModel>();
    if (!baseOcc || !baseOcc->getShape() || baseOcc->getShape()->IsNull()) return false;

    BRep_Builder builder;
    TopoDS_Compound comp;
    builder.MakeCompound(comp);
    for (TopoDS_Iterator it(*baseOcc->getShape()); it.More(); it.Next())
        builder.Add(comp, it.Value());
    for (const FC::VirtualShape& vs : ids) {
        if (vs.CmdId == baseVs.CmdId) continue;
        TopoDS_Shape s = getShapeFromSource(vs);
        if (s.IsNull()) return false;
        builder.Add(comp, s);
    }
    _occShapeAgent->updateShape(comp);
    return true;
}

FC::FCGeoEnum::FCGeometryComType FCOCCModelOperRemoveCompound::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTCompoundRemoveShape;
}

bool FCOCCModelOperRemoveCompound::update()
{
    const QList<FC::VirtualShape>& toRemove = getShapeCmdIds();
    const FC::VirtualShape& compoundVs = getInputCmdId();
    if (toRemove.isEmpty() || compoundVs.CmdId <= 0) return false;

    FC::FCAbsGeoCommand* compoundCmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(compoundVs.CmdId);
    if (!compoundCmd) return false;
    FCAbstractOCCModel* compoundOcc = compoundCmd->getTShapeAgent<FCAbstractOCCModel>();
    if (!compoundOcc || !compoundOcc->getShape() || compoundOcc->getShape()->IsNull()) return false;

    BRep_Builder builder;
    TopoDS_Compound comp;
    builder.MakeCompound(comp);
    int idx = 0;
    for (TopoDS_Iterator it(*compoundOcc->getShape()); it.More(); it.Next(), ++idx) {
        bool skip = false;
        for (const FC::VirtualShape& vs : toRemove)
            if (vs.CmdId == compoundVs.CmdId && vs.VirtualTopoIndex == idx) { skip = true; break; }
        if (!skip) builder.Add(comp, it.Value());
    }
    _occShapeAgent->updateShape(comp);
    return true;
}

} // namespace OCC
