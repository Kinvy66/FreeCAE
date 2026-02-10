/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelImportedPart.h
 * @brief OCC 导入部件命令（移植自 FITKGeoCompOCC FITKOCCModelImportedPart）
 */
#ifndef FCOCCMODELIMPORTEDPART_H
#define FCOCCMODELIMPORTEDPART_H

#include "FCAbstractOCCModel.h"
#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoImportedPart.h>

namespace OCC {

class FCGEOMETRYCMD_API FCOCCModelImportedPart : public FC::FCGeoImportedPart, public OCCShapeAgent
{
public:
    FCOCCModelImportedPart();
    ~FCOCCModelImportedPart() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace OCC

#endif // FCOCCMODELIMPORTEDPART_H
