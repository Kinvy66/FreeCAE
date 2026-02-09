/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCGeoSketchDeleter.h
 * @brief 草图删除命令 OCC 实现（移植自 FITKOCCGeoSketchDeleter）
 */
#ifndef FCOCCGEOSKETCHDELETER_H
#define FCOCCGEOSKETCHDELETER_H

#include "FCAbstractOCCModel.h"
#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoSketchDeleter.h>

namespace OCC {

class FCGEOMETRYCMD_API FCOCCGeoSketchDeleter : public FC::FCGeoSketchDeleter, public OCCShapeAgent
{
public:
    FCOCCGeoSketchDeleter() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCGeoSketchDeleter() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace OCC

#endif // FCOCCGEOSKETCHDELETER_H
