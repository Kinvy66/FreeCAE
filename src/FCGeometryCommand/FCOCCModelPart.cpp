/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

#include "FCOCCModelPart.h"
#include "FCAbstractOCCModel.h"
#include <TopoDS_Shape.hxx>

namespace OCC {

FCOCCModelPart::FCOCCModelPart()
    : OCCShapeAgent(this)
{
    _shapeAgent = _occShapeAgent;
}

FC::FCGeoEnum::FCGeometryComType FCOCCModelPart::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTPart;
}

bool FCOCCModelPart::update()
{
    int n = getDataCount();
    if (n <= 0) {
        _occShapeAgent->updateShape(TopoDS_Shape());
        _occShapeAgent->buildVirtualTopo(false);
        _occShapeAgent->triangulation();
        return true;
    }
    FC::FCAbsGeoCommand* lastCmd = getDataByIndex(n - 1);
    if (!lastCmd) {
        _occShapeAgent->updateShape(TopoDS_Shape());
        return true;
    }
    FCAbstractOCCModel* occModel = lastCmd->getTShapeAgent<FCAbstractOCCModel>();
    if (!occModel || !occModel->getShape() || occModel->getShape()->IsNull()) {
        _occShapeAgent->updateShape(TopoDS_Shape());
        return true;
    }
    _occShapeAgent->updateShape(*occModel->getShape());
    _occShapeAgent->buildVirtualTopo(false);
    _occShapeAgent->triangulation();
    return true;
}

} // namespace OCC
