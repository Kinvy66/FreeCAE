/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelOperCompound.h
 * @brief 复合体操作命令 OCC 实现（移植自 FITKOCCModelOperCompound）
 */
#ifndef FCOCCMODELOPERCOMPOUND_H
#define FCOCCMODELOPERCOMPOUND_H

#include "FCAbstractOCCModel.h"
#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoModelOperCompound.h>

namespace OCC {

class FCGEOMETRYCMD_API FCOCCModelOperAddCompound : public FC::FCGeoModelOperCompound, public OCCShapeAgent
{
public:
    FCOCCModelOperAddCompound() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelOperAddCompound() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelOperRemoveCompound : public FC::FCGeoModelOperCompound, public OCCShapeAgent
{
public:
    FCOCCModelOperRemoveCompound() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelOperRemoveCompound() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace OCC

#endif // FCOCCMODELOPERCOMPOUND_H
