/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCSplitter.h
 * @brief OCC 分割命令（曲线/面/体）（移植自 FITKGeoCompOCC FITKOCCSplitter）
 */
#ifndef FCOCCSPLITTER_H
#define FCOCCSPLITTER_H

#include "FCAbstractOCCModel.h"
#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoSplitter.h>

namespace OCC {

class FCGEOMETRYCMD_API FCOCCCurveSplitter : public FC::FCGeoCurveSplitter, public OCCShapeAgent
{
public:
    FCOCCCurveSplitter();
    ~FCOCCCurveSplitter() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCSurfaceSplitter : public FC::FCGeoSurfaceSplitter, public OCCShapeAgent
{
public:
    FCOCCSurfaceSplitter();
    ~FCOCCSurfaceSplitter() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCSolidSplitter : public FC::FCGeoSolidSplitter, public OCCShapeAgent
{
public:
    FCOCCSolidSplitter();
    ~FCOCCSolidSplitter() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace OCC

#endif // FCOCCSPLITTER_H
