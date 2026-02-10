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

/** 曲线端点（移植自 FITKOCCModelCurveEnd） */
class FCGEOMETRYCMD_API FCOCCModelCurveEnd : public FC::FCGeoModelCurveEnd, public OCCShapeAgent
{
public:
    FCOCCModelCurveEnd();
    ~FCOCCModelCurveEnd() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

/** 曲线上距端点指定距离的点（移植自 FITKOCCModelCurveEndDistance） */
class FCGEOMETRYCMD_API FCOCCModelCurveEndDistance : public FC::FCGeoModelCurveEndDistance, public OCCShapeAgent
{
public:
    FCOCCModelCurveEndDistance();
    ~FCOCCModelCurveEndDistance() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

/** 曲线上比例参数点（移植自 FITKOCCModelCurveRadio） */
class FCGEOMETRYCMD_API FCOCCModelCurveRadio : public FC::FCGeoModelCurveRadio, public OCCShapeAgent
{
public:
    FCOCCModelCurveRadio();
    ~FCOCCModelCurveRadio() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

/** 点在曲线上的投影（移植自 FITKOCCModelPointProjectionOnCurve） */
class FCGEOMETRYCMD_API FCOCCModelPointProjectionOnCurve : public FC::FCGeoModelPointProjectionOnCurve, public OCCShapeAgent
{
public:
    FCOCCModelPointProjectionOnCurve();
    ~FCOCCModelPointProjectionOnCurve() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

/** 两曲线交点（移植自 FITKOCCModelTwoCurveIntersection） */
class FCGEOMETRYCMD_API FCOCCModelTwoCurveIntersectPoint : public FC::FCGeoModelTwoCurveIntersectPoint, public OCCShapeAgent
{
public:
    FCOCCModelTwoCurveIntersectPoint();
    ~FCOCCModelTwoCurveIntersectPoint() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

/** 点在曲面上的投影（移植自 FITKOCCModelPointProjectionOnSurface） */
class FCGEOMETRYCMD_API FCOCCModelPointProjectionOnSurface : public FC::FCGeoModelPointProjectionOnSurface, public OCCShapeAgent
{
public:
    FCOCCModelPointProjectionOnSurface();
    ~FCOCCModelPointProjectionOnSurface() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

/** 曲线与曲面的交点（移植自 FITKOCCModelCurveIntersectionSurface） */
class FCGEOMETRYCMD_API FCOCCModelCurveIntersectSurface : public FC::FCGeoModelCurveIntersectSurface, public OCCShapeAgent
{
public:
    FCOCCModelCurveIntersectSurface();
    ~FCOCCModelCurveIntersectSurface() override = default;
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
