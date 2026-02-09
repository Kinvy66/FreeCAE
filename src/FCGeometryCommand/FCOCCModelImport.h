/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelImport.h
 * @brief OCC 几何导入（移植自 FITKGeoCompOCC FITKOCCModelImport）
 */
#ifndef FCOCCMODELIMPORT_H
#define FCOCCMODELIMPORT_H

#include "FCAbstractOCCModel.h"
#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoModelImport.h>

namespace OCC {

class FCGEOMETRYCMD_API FCOCCModelImport : public FC::FCGeoModelImport, public OCCShapeAgent
{
public:
    explicit FCOCCModelImport();
    ~FCOCCModelImport() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace OCC

#endif // FCOCCMODELIMPORT_H
