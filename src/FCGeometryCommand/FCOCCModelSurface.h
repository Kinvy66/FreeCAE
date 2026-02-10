/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelSurface.h
 * @brief OCC 曲面命令（移植自 FITKGeoCompOCC FITKOCCModelSurface，除 CoverEdges 外）
 */
#ifndef FCOCCMODELSURFACE_H
#define FCOCCMODELSURFACE_H

#include "FCAbstractOCCModel.h"
#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoModelSurface.h>

namespace OCC {

/** 封闭线框成面（移植自 FITKOCCModelClosedWireSurface） */
class FCGEOMETRYCMD_API FCOCCModelClosedWireSurface : public FC::FCGeoModelClosedWireSurface, public OCCShapeAgent
{
public:
    FCOCCModelClosedWireSurface();
    ~FCOCCModelClosedWireSurface() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

/** 偏移曲面 */
class FCGEOMETRYCMD_API FCOCCModelOffsetSurface : public FC::FCGeoModelOffsetSurface, public OCCShapeAgent
{
public:
    FCOCCModelOffsetSurface();
    ~FCOCCModelOffsetSurface() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

/** 拉伸曲面 */
class FCGEOMETRYCMD_API FCOCCModelExtrudeSurface : public FC::FCGeoModelExtrudeSurface, public OCCShapeAgent
{
public:
    FCOCCModelExtrudeSurface();
    ~FCOCCModelExtrudeSurface() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

/** 旋转曲面 */
class FCGEOMETRYCMD_API FCOCCModelRevolSurface : public FC::FCGeoModelRevolSurface, public OCCShapeAgent
{
public:
    FCOCCModelRevolSurface();
    ~FCOCCModelRevolSurface() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

/** 扫略曲面（路径+截面 Wire，BRepOffsetAPI_MakePipeShell） */
class FCGEOMETRYCMD_API FCOCCModelSweepSurface : public FC::FCGeoModelSweepSurface, public OCCShapeAgent
{
public:
    FCOCCModelSweepSurface();
    ~FCOCCModelSweepSurface() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

/** 多截面曲面（BRepOffsetAPI_ThruSections） */
class FCGEOMETRYCMD_API FCOCCModelMultiSectionSurface : public FC::FCGeoModelMultiSectionSurface, public OCCShapeAgent
{
public:
    FCOCCModelMultiSectionSurface();
    ~FCOCCModelMultiSectionSurface() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

/** 桥接曲面（两面两边，BRepFill_Filling） */
class FCGEOMETRYCMD_API FCOCCModelBridgeSurface : public FC::FCGeoModelBridgeSurface, public OCCShapeAgent
{
public:
    FCOCCModelBridgeSurface();
    ~FCOCCModelBridgeSurface() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

/** 体的面（从实体取面） */
class FCGEOMETRYCMD_API FCOCCModelSolidSurface : public FC::FCGeoModelSolidSurface, public OCCShapeAgent
{
public:
    FCOCCModelSolidSurface();
    ~FCOCCModelSolidSurface() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace OCC

#endif // FCOCCMODELSURFACE_H
