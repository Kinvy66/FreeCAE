/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelPartitionSolid.h
 * @brief OCC 体分块命令（移植自 FITKGeoCompOCC FITKOCCModelPartitionSolid）
 */
#ifndef FCOCCMODELPARTITIONSOLID_H
#define FCOCCMODELPARTITIONSOLID_H

#include "FCAbstractOCCModel.h"
#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoModelPartitionSolid.h>

namespace OCC {

class FCGEOMETRYCMD_API FCOCCModelPartitionSolidWithPlane : public FC::FCGeoModelPartitionSolidWithPlane, public OCCShapeAgent
{
public:
    FCOCCModelPartitionSolidWithPlane();
    ~FCOCCModelPartitionSolidWithPlane() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelPartitionSolidWithExtendFace : public FC::FCGeoModelPartitionSolidWithExtendFace, public OCCShapeAgent
{
public:
    FCOCCModelPartitionSolidWithExtendFace();
    ~FCOCCModelPartitionSolidWithExtendFace() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelPartitionSolidWithSweepEdge : public FC::FCGeoModelPartitionSolidWithSweepEdge, public OCCShapeAgent
{
public:
    FCOCCModelPartitionSolidWithSweepEdge();
    ~FCOCCModelPartitionSolidWithSweepEdge() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelPartitionSolidWithSketchPlanar : public FC::FCGeoModelPartitionSolidWithSketchPlanar, public OCCShapeAgent
{
public:
    FCOCCModelPartitionSolidWithSketchPlanar();
    ~FCOCCModelPartitionSolidWithSketchPlanar() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace OCC

#endif // FCOCCMODELPARTITIONSOLID_H
