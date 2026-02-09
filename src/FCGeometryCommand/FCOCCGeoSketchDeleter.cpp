/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCGeoSketchDeleter.cpp
 * @brief 草图删除命令 OCC 实现（移植自 FITKGeoCompOCC FITKOCCGeoSketchDeleter）
 * 从各命令形状中移除指定边/点拓扑，不依赖 Sketch2D。
 */
#include "FCOCCGeoSketchDeleter.h"
#include "FCAbstractOCCModel.h"
#include <FCDataRepo.h>
#include <FCGeometryInterface/FCGeoEnum.h>
#include <FCModelInterface/FCModelEnum.h>

#include <BRepTools_ReShape.hxx>
#include <TopoDS_Shape.hxx>

#include <QHash>

namespace OCC {

FC::FCGeoEnum::FITKGeometryComType FCOCCGeoSketchDeleter::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTSketchDeleter;
}

bool FCOCCGeoSketchDeleter::update()
{
    QList<FC::VirtualShape> entities = toBeRemovedEntities();
    if (entities.isEmpty()) return false;

    BRepTools_ReShape reShape;
    QHash<FCAbstractOCCModel*, TopoDS_Shape> affected; // model -> original shape (by value)

    for (const FC::VirtualShape& entity : entities)
    {
        FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(entity.CmdId);
        if (!cmd) return false;
        FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
        if (!occ || !occ->getShape() || occ->getShape()->IsNull()) return false;

        TopoDS_Shape sub;
        sub = occ->getShape(FC::FCModelEnum::FMSEdge, entity.VirtualTopoId);
        if (sub.IsNull())
            sub = occ->getShape(FC::FCModelEnum::FMSPoint, entity.VirtualTopoId);
        if (sub.IsNull()) return false;

        reShape.Remove(sub);
        if (!affected.contains(occ))
            affected.insert(occ, *occ->getShape());
    }

    for (auto it = affected.begin(); it != affected.end(); ++it)
    {
        TopoDS_Shape newShape = reShape.Apply(it.value());
        if (!newShape.IsNull())
            it.key()->updateShape(newShape);
    }
    return true;
}

} // namespace OCC
