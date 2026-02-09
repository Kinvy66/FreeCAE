/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelImportFlattenAssembly.h
 * @brief 扁平装配导入 OCC 实现（移植自 FITKOCCModelImportFlattenAssembly）
 */
#ifndef FCOCCMODELIMPORTFLATTENASSEMBLY_H
#define FCOCCMODELIMPORTFLATTENASSEMBLY_H

#include "FCAbstractOCCModel.h"
#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoModelImportFlattenAssembly.h>

namespace OCC {

class FCGEOMETRYCMD_API FCOCCModelImportFlattenAssembly : public FC::FCGeoModelImportFlattenAssembly, public OCCShapeAgent
{
public:
    FCOCCModelImportFlattenAssembly() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelImportFlattenAssembly() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace OCC

#endif // FCOCCMODELIMPORTFLATTENASSEMBLY_H
