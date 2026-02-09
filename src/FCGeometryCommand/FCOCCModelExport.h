/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelExport.h
 * @brief OCC 几何导出（移植自 FITKGeoCompOCC FITKOCCModelExport）
 */
#ifndef FCOCCMODELEXPORT_H
#define FCOCCMODELEXPORT_H

#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoModelExport.h>

namespace OCC {

class FCGEOMETRYCMD_API FCOCCModelExport : public FC::FCGeoModelExport
{
public:
    explicit FCOCCModelExport() = default;
    ~FCOCCModelExport() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace OCC

#endif // FCOCCMODELEXPORT_H
