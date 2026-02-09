/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCStiffener.h
 * @brief 肋特征命令 OCC 实现（移植自 FITKOCCFeature FITKOCCStiffener）
 */
#ifndef FCOCCSTIFFENER_H
#define FCOCCSTIFFENER_H

#include "FCAbstractOCCModel.h"
#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCGeoStiffener.h>

namespace OCC {

class FCGEOMETRYCMD_API FCOCCStiffener : public FC::FCGeoStiffener, public OCCShapeAgent
{
public:
    FCOCCStiffener() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCStiffener() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace OCC

#endif // FCOCCSTIFFENER_H
