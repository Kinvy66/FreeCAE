/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelExtractCenter.h
 * @brief 抽圆心/球心命令 OCC 实现（移植自 FITKOCCModelExtractCenter）
 */
#ifndef FCOCCMODELEXTRACTCENTER_H
#define FCOCCMODELEXTRACTCENTER_H

#include "FCAbstractOCCModel.h"
#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoModelExtractCenter.h>

namespace OCC {

class FCGEOMETRYCMD_API FCOCCModelExtractCenter : public FC::FCGeoModelExtractCenter, public OCCShapeAgent
{
public:
    FCOCCModelExtractCenter() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelExtractCenter() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace OCC

#endif // FCOCCMODELEXTRACTCENTER_H
