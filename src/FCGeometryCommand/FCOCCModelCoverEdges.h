/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelCoverEdges.h
 * @brief 覆盖面命令 OCC 实现（移植自 FITKOCCModelCoverEdges）
 */
#ifndef FCOCCMODELCOVEREDGES_H
#define FCOCCMODELCOVEREDGES_H

#include "FCAbstractOCCModel.h"
#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoCoverEdges.h>

namespace OCC {

class FCGEOMETRYCMD_API FCOCCModelCoverEdges : public FC::FCGeoCoverEdges, public OCCShapeAgent
{
public:
    FCOCCModelCoverEdges() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelCoverEdges() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace OCC

#endif // FCOCCMODELCOVEREDGES_H
