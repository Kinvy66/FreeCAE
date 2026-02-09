/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelSimpleShape.h
 * @brief OCC 基本体（盒、圆柱、球）（移植自 FITKGeoCompOCC FITKOCCModelSimpleShape）
 */
#ifndef FCOCCMODELSIMPLESHAPE_H
#define FCOCCMODELSIMPLESHAPE_H

#include "FCAbstractOCCModel.h"
#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoModelBox.h>
#include <FCGeometryInterface/FCAbsGeoModelCylinder.h>
#include <FCGeometryInterface/FCAbsGeoModelSphere.h>

namespace OCC {

class FCGEOMETRYCMD_API FCOCCModelBox : public FC::FCGeoModelBox, public OCCShapeAgent
{
public:
    explicit FCOCCModelBox();
    ~FCOCCModelBox() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelCylinder : public FC::FCGeoModelCylinder, public OCCShapeAgent
{
public:
    explicit FCOCCModelCylinder();
    ~FCOCCModelCylinder() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelSphere : public FC::FCGeoModelSphere, public OCCShapeAgent
{
public:
    explicit FCOCCModelSphere();
    ~FCOCCModelSphere() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace OCC

#endif // FCOCCMODELSIMPLESHAPE_H
