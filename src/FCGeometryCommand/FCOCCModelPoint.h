/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelPoint.h
 * @brief OCC 点命令（移植自 FITKGeoCompOCC FITKOCCModelPoint）
 */
#ifndef FCOCCMODELPOINT_H
#define FCOCCMODELPOINT_H

#include "FCAbstractOCCModel.h"
#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoModelPoint.h>

namespace OCC {

class FCGEOMETRYCMD_API FCOCCModelPoint : public FC::FCGeoModelPoint, public OCCShapeAgent
{
public:
    FCOCCModelPoint();
    ~FCOCCModelPoint() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

/** 交互点（线/面上拾取，update 暂为桩） */
class FCGEOMETRYCMD_API FCOCCModelInteractionPoint : public FC::FCGeoModelInteractionPoint, public OCCShapeAgent
{
public:
    FCOCCModelInteractionPoint();
    ~FCOCCModelInteractionPoint() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace OCC

#endif // FCOCCMODELPOINT_H
