/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelSolid.h
 * @brief OCC 实体命令（封闭曲面/拉伸/旋转/扫掠/多截面）（移植自 FITKGeoCompOCC FITKOCCModelSolid）
 */
#ifndef FCOCCMODELSOLID_H
#define FCOCCMODELSOLID_H

#include "FCAbstractOCCModel.h"
#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoModelSolid.h>

namespace OCC {

class FCGEOMETRYCMD_API FCOCCModelClosedSurfaceSolid : public FC::FCAbsGeoModelClosedSurfaceSolid, public OCCShapeAgent
{
public:
    FCOCCModelClosedSurfaceSolid();
    ~FCOCCModelClosedSurfaceSolid() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelExtrudeSolid : public FC::FCAbsGeoModelExtrudeSolid, public OCCShapeAgent
{
public:
    FCOCCModelExtrudeSolid();
    ~FCOCCModelExtrudeSolid() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelRevolSolid : public FC::FCAbsGeoModelRevolSolid, public OCCShapeAgent
{
public:
    FCOCCModelRevolSolid();
    ~FCOCCModelRevolSolid() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelSweepSolid : public FC::FCAbsGeoModelSweepSolid, public OCCShapeAgent
{
public:
    FCOCCModelSweepSolid();
    ~FCOCCModelSweepSolid() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelMultiSectionSolid : public FC::FCAbsGeoModelMultiSectionSolid, public OCCShapeAgent
{
public:
    FCOCCModelMultiSectionSolid();
    ~FCOCCModelMultiSectionSolid() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace OCC

#endif // FCOCCMODELSOLID_H
