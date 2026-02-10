/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCSampleCurve.h
 * @brief OCC 曲线采样命令（移植自 FITKGeoCompOCC FITKOCCSampleCurve）
 */
#ifndef FCOCCSAMPLECURVE_H
#define FCOCCSAMPLECURVE_H

#include "FCAbstractOCCModel.h"
#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoSampleCurve.h>

namespace OCC {

/**
 * @brief OCC 曲线采样（等参/等弧长/等弦长），输出为顶点 Compound
 */
class FCGEOMETRYCMD_API FCOCCSampleCurve : public FC::FCGeoSampleCurve, public OCCShapeAgent
{
public:
    explicit FCOCCSampleCurve();
    ~FCOCCSampleCurve() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace OCC

#endif // FCOCCSAMPLECURVE_H
