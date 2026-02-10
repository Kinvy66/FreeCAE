/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelCurve.h
 * @brief OCC 曲线命令（移植自 FITKGeoCompOCC FITKOCCModelCurve）
 */
#ifndef FCOCCMODELCURVE_H
#define FCOCCMODELCURVE_H

#include "FCAbstractOCCModel.h"
#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoModelCurve.h>

namespace OCC {

class FCGEOMETRYCMD_API FCOCCModelLine : public FC::FCGeoModelLine, public OCCShapeAgent
{
public:
    FCOCCModelLine();
    ~FCOCCModelLine() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelSegment : public FC::FCGeoModelSegment, public OCCShapeAgent
{
public:
    FCOCCModelSegment();
    ~FCOCCModelSegment() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelWire : public FC::FCGeoModelWire, public OCCShapeAgent
{
public:
    FCOCCModelWire();
    ~FCOCCModelWire() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelCircle : public FC::FCGeoModelCircle, public OCCShapeAgent
{
public:
    FCOCCModelCircle();
    ~FCOCCModelCircle() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelThreePointsCircle : public FC::FCGeoModelThreePointsCircle, public OCCShapeAgent
{
public:
    FCOCCModelThreePointsCircle();
    ~FCOCCModelThreePointsCircle() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelCircleArc : public FC::FCGeoModelCircleArc, public OCCShapeAgent
{
public:
    FCOCCModelCircleArc();
    ~FCOCCModelCircleArc() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelEllipse : public FC::FCGeoModelEllipse, public OCCShapeAgent
{
public:
    FCOCCModelEllipse();
    ~FCOCCModelEllipse() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelThreePointsEllipse : public FC::FCGeoModelThreePointsEllipse, public OCCShapeAgent
{
public:
    FCOCCModelThreePointsEllipse();
    ~FCOCCModelThreePointsEllipse() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelEllipseArc : public FC::FCGeoModelEllipseArc, public OCCShapeAgent
{
public:
    FCOCCModelEllipseArc();
    ~FCOCCModelEllipseArc() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelHyperbola : public FC::FCGeoModelHyperbola, public OCCShapeAgent
{
public:
    FCOCCModelHyperbola();
    ~FCOCCModelHyperbola() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelThreePointsHyperbola : public FC::FCGeoModelThreePointsHyperbola, public OCCShapeAgent
{
public:
    FCOCCModelThreePointsHyperbola();
    ~FCOCCModelThreePointsHyperbola() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelHyperbolaArc : public FC::FCGeoModelHyperbolaArc, public OCCShapeAgent
{
public:
    FCOCCModelHyperbolaArc();
    ~FCOCCModelHyperbolaArc() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelParabola : public FC::FCGeoModelParabola, public OCCShapeAgent
{
public:
    FCOCCModelParabola();
    ~FCOCCModelParabola() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelThreePointsParabola : public FC::FCGeoModelThreePointsParabola, public OCCShapeAgent
{
public:
    FCOCCModelThreePointsParabola();
    ~FCOCCModelThreePointsParabola() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelParabolaArc : public FC::FCGeoModelParabolaArc, public OCCShapeAgent
{
public:
    FCOCCModelParabolaArc();
    ~FCOCCModelParabolaArc() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelBezierByControlPoints : public FC::FCGeoModelBezierByControlPoints, public OCCShapeAgent
{
public:
    FCOCCModelBezierByControlPoints();
    ~FCOCCModelBezierByControlPoints() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelBsplineByThroughPoints : public FC::FCGeoModelBsplineByThroughPoints, public OCCShapeAgent
{
public:
    FCOCCModelBsplineByThroughPoints();
    ~FCOCCModelBsplineByThroughPoints() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelTrimmedCurve : public FC::FCGeoModelTrimmedCurve, public OCCShapeAgent
{
public:
    FCOCCModelTrimmedCurve();
    ~FCOCCModelTrimmedCurve() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelOffsetCurve : public FC::FCGeoModelOffsetCurve, public OCCShapeAgent
{
public:
    FCOCCModelOffsetCurve();
    ~FCOCCModelOffsetCurve() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelCurveProjectionOnSurface : public FC::FCGeoModelCurveProjectionOnSurface, public OCCShapeAgent
{
public:
    FCOCCModelCurveProjectionOnSurface();
    ~FCOCCModelCurveProjectionOnSurface() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelSurfaceIntersectionSurface : public FC::FCGeoModelSurfaceIntersectionSurface, public OCCShapeAgent
{
public:
    FCOCCModelSurfaceIntersectionSurface();
    ~FCOCCModelSurfaceIntersectionSurface() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelBridgeCurve : public FC::FCGeoModelBridgeCurve, public OCCShapeAgent
{
public:
    FCOCCModelBridgeCurve();
    ~FCOCCModelBridgeCurve() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelSurfaceEdge : public FC::FCGeoModelSurfaceEdge, public OCCShapeAgent
{
public:
    FCOCCModelSurfaceEdge();
    ~FCOCCModelSurfaceEdge() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace OCC

#endif // FCOCCMODELCURVE_H
