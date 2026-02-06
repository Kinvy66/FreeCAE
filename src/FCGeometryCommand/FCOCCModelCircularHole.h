/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelCircularHole.h
 * @brief OCC 打圆孔（与 FreeCAE FCGeoModelCircularHoleSolid 接口关联）
 */
#ifndef FCOCCMODELCIRCULARHOLE_H
#define FCOCCMODELCIRCULARHOLE_H

#include "FCAbstractOCCModel.h"
#include "FCAbsGeoModelOperSolid.h"
#include "FCGeometryCommandAPI.h"

class TopoDS_Shape;

namespace OCC
{
    class FCGEOMETRYCMD_API FCOCCModelCircularHole : public FC::FCAbsGeoModelCircularHoleSolid, public OCCShapeAgent
    {
    public:
        FCOCCModelCircularHole();
        ~FCOCCModelCircularHole() override = default;
        bool update() override;
    };
}

#endif
