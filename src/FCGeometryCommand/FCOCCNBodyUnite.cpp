/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

#include "FCOCCNBodyUnite.h"
#include <FCData/FCDataRepo.h>
#include <BRepAlgoAPI_Fuse.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Iterator.hxx>
#include <TopExp_Explorer.hxx>
#include <TopAbs_ShapeEnum.hxx>

namespace OCC {

FCOCCNBodyUnite::FCOCCNBodyUnite()
    : OCCShapeAgent(this)
{
    _shapeAgent = _occShapeAgent;
}

FC::FCGeoEnum::FITKGeometryComType FCOCCNBodyUnite::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTNBodyUnite;
}

bool FCOCCNBodyUnite::update()
{
    const QList<FC::VirtualShape>& shapes = getShapes();
    if (shapes.size() < 2) return false;
    FC::FCDataRepo* repo = FC::FCDataRepo::instance();
    TopoDS_Shape acc;
    for (const FC::VirtualShape& vs : shapes) {
        if (vs.CmdId <= 0) continue;
        FC::FCAbsGeoCommand* cmd = repo->getDataAs<FC::FCAbsGeoCommand>(vs.CmdId);
        if (!cmd) continue;
        OCC::FCAbstractOCCModel* occModel = cmd->getTShapeAgent<OCC::FCAbstractOCCModel>();
        if (!occModel || !occModel->getShape() || occModel->getShape()->IsNull()) continue;
        const TopoDS_Shape& s = *occModel->getShape();
        if (acc.IsNull())
            acc = s;
        else {
            try {
                acc = BRepAlgoAPI_Fuse(acc, s).Shape();
            } catch (...) {
                return false;
            }
        }
    }
    if (acc.IsNull()) return false;
    _occShapeAgent->updateShape(acc);
    return true;
}

} // namespace OCC
