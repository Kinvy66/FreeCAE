/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelReplaceFaces.h
 * @brief 替换面命令 OCC 实现（移植自 FITKOCCModelReplaceFaces）
 */
#ifndef FCOCCMODELREPLACEFACES_H
#define FCOCCMODELREPLACEFACES_H

#include "FCAbstractOCCModel.h"
#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoReplaceFaces.h>

namespace OCC {

class FCGEOMETRYCMD_API FCOCCModelReplaceFaces : public FC::FCGeoReplaceFaces, public OCCShapeAgent
{
public:
    FCOCCModelReplaceFaces() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelReplaceFaces() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace OCC

#endif // FCOCCMODELREPLACEFACES_H
