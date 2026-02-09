/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelRepairSmallFaces.h
 * @brief 修复小面命令 OCC 实现（移植自 FITKOCCModelRepairSmallFaces）
 */
#ifndef FCOCCMODELREPAIRSMALLFACES_H
#define FCOCCMODELREPAIRSMALLFACES_H

#include "FCAbstractOCCModel.h"
#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoRepairSmallFaces.h>

namespace OCC {

class FCGEOMETRYCMD_API FCOCCModelRepairSmallFaces : public FC::FCGeoRepairSmallFaces, public OCCShapeAgent
{
public:
    FCOCCModelRepairSmallFaces() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelRepairSmallFaces() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace OCC

#endif // FCOCCMODELREPAIRSMALLFACES_H
