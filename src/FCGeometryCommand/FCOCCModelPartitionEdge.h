/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelPartitionEdge.h
 * @brief OCC 边分块命令（移植自 FITKGeoCompOCC FITKOCCModelPartitionEdge）
 */
#ifndef FCOCCMODELPARTITIONEDGE_H
#define FCOCCMODELPARTITIONEDGE_H

#include "FCAbstractOCCModel.h"
#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoModelPartitionEdge.h>

namespace OCC {

class FCGEOMETRYCMD_API FCOCCModelPartitionEdgeWithParameter : public FC::FCGeoModelPartitionEdgeWithParameter, public OCCShapeAgent
{
public:
    FCOCCModelPartitionEdgeWithParameter();
    ~FCOCCModelPartitionEdgeWithParameter() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelPartitionEdgeWithPoint : public FC::FCGeoModelPartitionEdgeWithPoint, public OCCShapeAgent
{
public:
    FCOCCModelPartitionEdgeWithPoint();
    ~FCOCCModelPartitionEdgeWithPoint() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelPartitionEdgeWithDatumPlane : public FC::FCGeoModelPartitionEdgeWithDatumPlane, public OCCShapeAgent
{
public:
    FCOCCModelPartitionEdgeWithDatumPlane();
    ~FCOCCModelPartitionEdgeWithDatumPlane() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace OCC

#endif // FCOCCMODELPARTITIONEDGE_H
