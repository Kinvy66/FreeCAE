/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelFromMesh.h
 * @brief 从网格生成几何命令 OCC 实现（移植自 FITKOCCModelFromMesh）
 */
#ifndef FCOCCMODELFROMMESH_H
#define FCOCCMODELFROMMESH_H

#include "FCAbstractOCCModel.h"
#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoModelFromMesh.h>

namespace OCC {

class FCGEOMETRYCMD_API FCOCCModelFromMesh : public FC::FCGeoModelFromMesh, public OCCShapeAgent
{
public:
    FCOCCModelFromMesh() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelFromMesh() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace OCC

#endif // FCOCCMODELFROMMESH_H
