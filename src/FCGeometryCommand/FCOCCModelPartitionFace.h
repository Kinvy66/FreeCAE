/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelPartitionFace.h
 * @brief OCC 面分块命令（移植自 FITKGeoCompOCC FITKOCCModelPartitionFace）
 */
#ifndef FCOCCMODELPARTITIONFACE_H
#define FCOCCMODELPARTITIONFACE_H

#include "FCAbstractOCCModel.h"
#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoModelPartitionFace.h>

namespace OCC {

class FCGEOMETRYCMD_API FCOCCModelPartitionFaceWithSketch : public FC::FCGeoModelPartitionFaceWithSketch, public OCCShapeAgent
{
public:
    FCOCCModelPartitionFaceWithSketch();
    ~FCOCCModelPartitionFaceWithSketch() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelPartitionFaceWithTwoPoints : public FC::FCGeoModelPartitionFaceWithTwoPoints, public OCCShapeAgent
{
public:
    FCOCCModelPartitionFaceWithTwoPoints();
    ~FCOCCModelPartitionFaceWithTwoPoints() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelPartitionFaceWithDatumPlane : public FC::FCGeoModelPartitionFaceWithDatumPlane, public OCCShapeAgent
{
public:
    FCOCCModelPartitionFaceWithDatumPlane();
    ~FCOCCModelPartitionFaceWithDatumPlane() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelPartitionFaceWithCurvedPath : public FC::FCGeoModelPartitionFaceWithCurvedPath, public OCCShapeAgent
{
public:
    FCOCCModelPartitionFaceWithCurvedPath();
    ~FCOCCModelPartitionFaceWithCurvedPath() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelPartitionFaceWithExtendFace : public FC::FCGeoModelPartitionFaceWithExtendFace, public OCCShapeAgent
{
public:
    FCOCCModelPartitionFaceWithExtendFace();
    ~FCOCCModelPartitionFaceWithExtendFace() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelPartitionFaceWithIntersectFace : public FC::FCGeoModelPartitionFaceWithIntersectFace, public OCCShapeAgent
{
public:
    FCOCCModelPartitionFaceWithIntersectFace();
    ~FCOCCModelPartitionFaceWithIntersectFace() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelPartitionFaceWithProjectEdges : public FC::FCGeoModelPartitionFaceWithProjectEdges, public OCCShapeAgent
{
public:
    FCOCCModelPartitionFaceWithProjectEdges();
    ~FCOCCModelPartitionFaceWithProjectEdges() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace OCC

#endif // FCOCCMODELPARTITIONFACE_H
