/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelCut.h
 * @brief OCC 切削命令（拉伸/旋转/扫掠/多截面）（移植自 FITKGeoCompOCC FITKOCCModelCut）
 */
#ifndef FCOCCMODELCUT_H
#define FCOCCMODELCUT_H

#include "FCAbstractOCCModel.h"
#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoModelCut.h>

namespace OCC {

class FCGEOMETRYCMD_API FCOCCModelExtrudeCut : public FC::FCAbsGeoModelExtrudeCut, public OCCShapeAgent
{
public:
    FCOCCModelExtrudeCut();
    ~FCOCCModelExtrudeCut() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelRevolCut : public FC::FCAbsGeoModelRevolCut, public OCCShapeAgent
{
public:
    FCOCCModelRevolCut();
    ~FCOCCModelRevolCut() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelSweepCut : public FC::FCAbsGeoModelSweepCut, public OCCShapeAgent
{
public:
    FCOCCModelSweepCut();
    ~FCOCCModelSweepCut() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelMultiSectionCut : public FC::FCAbsGeoModelMultiSectionCut, public OCCShapeAgent
{
public:
    FCOCCModelMultiSectionCut();
    ~FCOCCModelMultiSectionCut() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace OCC

#endif // FCOCCMODELCUT_H
