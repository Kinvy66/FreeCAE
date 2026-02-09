/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelExportTopos.h
 * @brief 导出几何拓扑命令 OCC 实现（移植自 FITKOCCModelExportTopos）
 */
#ifndef FCOCCMODELEXPORTTOPOS_H
#define FCOCCMODELEXPORTTOPOS_H

#include "FCAbstractOCCModel.h"
#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoModelExportTopos.h>

namespace OCC {

class FCGEOMETRYCMD_API FCOCCModelExportTopos : public FC::FCGeoModelExportTopos, public OCCShapeAgent
{
public:
    FCOCCModelExportTopos() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelExportTopos() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace OCC

#endif // FCOCCMODELEXPORTTOPOS_H
