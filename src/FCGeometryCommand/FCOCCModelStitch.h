/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelStitch.h
 * @brief 缝合面命令 OCC 实现（移植自 FITKOCCModelStitch）
 */
#ifndef FCOCCMODELSTITCH_H
#define FCOCCMODELSTITCH_H

#include "FCAbstractOCCModel.h"
#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoStitch.h>

namespace OCC {

class FCGEOMETRYCMD_API FCOCCModelStitch : public FC::FCGeoStitch, public OCCShapeAgent
{
public:
    FCOCCModelStitch() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelStitch() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace OCC

#endif // FCOCCMODELSTITCH_H
