/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelOperImprintSolid.h
 * @brief 压印体命令 OCC 实现（移植自 FITKOCCModelOperImprintSolid）
 */
#ifndef FCOCCMODELOPERIMPRINTSOLID_H
#define FCOCCMODELOPERIMPRINTSOLID_H

#include "FCAbstractOCCModel.h"
#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoModelOperImprintSolid.h>

namespace OCC {

class FCGEOMETRYCMD_API FCOCCModelOperImprintSolid : public FC::FCGeoModelOperImprintSolid, public OCCShapeAgent
{
public:
    FCOCCModelOperImprintSolid() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelOperImprintSolid() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace OCC

#endif // FCOCCMODELOPERIMPRINTSOLID_H
