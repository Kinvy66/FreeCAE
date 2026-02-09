/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCTransformation.h
 * @brief OCC 几何变换（平移/旋转/缩放/镜像）（移植自 FITKGeoCompOCC FITKOCCTransformation）
 */
#ifndef FCOCCTRANSFORMATION_H
#define FCOCCTRANSFORMATION_H

#include "FCAbstractOCCModel.h"
#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoTransformation.h>

namespace OCC {

class FCGEOMETRYCMD_API FCOCCModelTransform : public FC::FCGeoModelTransform, public OCCShapeAgent
{
public:
    FCOCCModelTransform();
    ~FCOCCModelTransform() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelTransformByTwoPoints : public FC::FCGeoModelTransformByTwoPoints, public OCCShapeAgent
{
public:
    FCOCCModelTransformByTwoPoints();
    ~FCOCCModelTransformByTwoPoints() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelTransformByDirAndDis : public FC::FCGeoModelTransformByDirAndDis, public OCCShapeAgent
{
public:
    FCOCCModelTransformByDirAndDis();
    ~FCOCCModelTransformByDirAndDis() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelRotate : public FC::FCGeoModelRotate, public OCCShapeAgent
{
public:
    FCOCCModelRotate();
    ~FCOCCModelRotate() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelScale : public FC::FCGeoModelScale, public OCCShapeAgent
{
public:
    FCOCCModelScale();
    ~FCOCCModelScale() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelMirror : public FC::FCGeoModelMirror, public OCCShapeAgent
{
public:
    FCOCCModelMirror();
    ~FCOCCModelMirror() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace OCC

#endif // FCOCCTRANSFORMATION_H
