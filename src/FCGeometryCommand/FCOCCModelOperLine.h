/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelOperLine.h
 * @brief 线操作命令 OCC 实现（移植自 FITKOCCModelOperLine）
 */
#ifndef FCOCCMODELOPERLINE_H
#define FCOCCMODELOPERLINE_H

#include "FCAbstractOCCModel.h"
#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoModelOperLine.h>

namespace OCC {

class FCGEOMETRYCMD_API FCOCCModelOperLine : public FC::FCGeoModelOperLine, public OCCShapeAgent
{
public:
    FCOCCModelOperLine() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelOperLine() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelMiddleOperLine : public FC::FCGeoModelOperLine, public OCCShapeAgent
{
public:
    FCOCCModelMiddleOperLine() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelMiddleOperLine() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace OCC

#endif // FCOCCMODELOPERLINE_H
