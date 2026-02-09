/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

#include "FCOCCOperBool.h"
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <TopoDS_Shape.hxx>

namespace OCC {

FCOCCOperBool::FCOCCOperBool()
    : OCCShapeAgent(this)
{
    _shapeAgent = _occShapeAgent;
}

FC::FCGeoEnum::FITKGeometryComType FCOCCOperBool::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTBool;
}

bool FCOCCOperBool::update()
{
    FC::FCAbsGeoShapeAgent* s1 = getGeoShape1();
    FC::FCAbsGeoShapeAgent* s2 = getGeoShape2();
    if (!s1 || !s2) return false;
    OCC::FCAbstractOCCModel* m1 = dynamic_cast<OCC::FCAbstractOCCModel*>(s1);
    OCC::FCAbstractOCCModel* m2 = dynamic_cast<OCC::FCAbstractOCCModel*>(s2);
    if (!m1 || !m2 || !m1->getShape() || !m2->getShape() || m1->getShape()->IsNull() || m2->getShape()->IsNull())
        return false;
    const TopoDS_Shape& a = *m1->getShape();
    const TopoDS_Shape& b = *m2->getShape();
    TopoDS_Shape result;
    try {
        switch (getBoolOperType()) {
        case FC::FCGeoOperBool::GBTCommon:
            result = BRepAlgoAPI_Common(a, b).Shape();
            break;
        case FC::FCGeoOperBool::GBTCut:
            result = BRepAlgoAPI_Cut(a, b).Shape();
            break;
        case FC::FCGeoOperBool::GBTAdd:
            result = BRepAlgoAPI_Fuse(a, b).Shape();
            break;
        default:
            return false;
        }
    } catch (...) {
        return false;
    }
    if (result.IsNull()) return false;
    _occShapeAgent->updateShape(result);
    return true;
}

} // namespace OCC
