/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelRepairSmallEdges.h
 * @brief 修复小边命令 OCC 实现（移植自 FITKOCCModelRepairSmallEdges）
 */
#ifndef FCOCCMODELREPAIRSMALLEDGES_H
#define FCOCCMODELREPAIRSMALLEDGES_H

#include "FCAbstractOCCModel.h"
#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoRepairSmallEdges.h>

namespace OCC {

class FCGEOMETRYCMD_API FCOCCModelRepairSmallEdges : public FC::FCGeoRepairSmallEdges, public OCCShapeAgent
{
public:
    FCOCCModelRepairSmallEdges() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelRepairSmallEdges() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace OCC

#endif // FCOCCMODELREPAIRSMALLEDGES_H
