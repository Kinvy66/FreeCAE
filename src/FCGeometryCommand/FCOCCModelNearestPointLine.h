/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelNearestPointLine.h
 * @brief 最近点线命令 OCC 实现（移植自 FITKOCCModelNearestPointLine）
 */
#ifndef FCOCCMODELNEARESTPOINTLINE_H
#define FCOCCMODELNEARESTPOINTLINE_H

#include "FCAbstractOCCModel.h"
#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoModelNearestPointLine.h>

namespace OCC {

class FCGEOMETRYCMD_API FCOCCModelNearestPointLine : public FC::FCGeoModelNearestPointLine, public OCCShapeAgent
{
public:
    FCOCCModelNearestPointLine() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelNearestPointLine() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace OCC

#endif // FCOCCMODELNEARESTPOINTLINE_H
