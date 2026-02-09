/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCOperBool.h
 * @brief OCC 布尔运算（交/并/差）（移植自 FITKGeoCompOCC FITKOCCOperBool）
 */
#ifndef FCOCCOPERBOOL_H
#define FCOCCOPERBOOL_H

#include "FCAbstractOCCModel.h"
#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoOperBool.h>

namespace OCC {

class FCGEOMETRYCMD_API FCOCCOperBool : public FC::FCGeoOperBool, public OCCShapeAgent
{
public:
    explicit FCOCCOperBool();
    ~FCOCCOperBool() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace OCC

#endif // FCOCCOPERBOOL_H
