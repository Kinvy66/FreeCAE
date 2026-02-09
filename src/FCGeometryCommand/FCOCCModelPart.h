/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelPart.h
 * @brief OCC 几何部件（移植自 FITKGeoCompOCC FITKOCCModelPart）
 */
#ifndef FCOCCMODELPART_H
#define FCOCCMODELPART_H

#include "FCAbstractOCCModel.h"
#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoPart.h>

namespace OCC {

class FCGEOMETRYCMD_API FCOCCModelPart : public FC::FCAbsGeoPart, public OCCShapeAgent
{
public:
    explicit FCOCCModelPart();
    ~FCOCCModelPart() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace OCC

#endif // FCOCCMODELPART_H
