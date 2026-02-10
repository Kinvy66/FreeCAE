/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelImportedPart.cpp
 * @brief OCC 导入部件命令实现（移植自 FITKGeoCompOCC FITKOCCModelImportedPart）
 */
#include "FCOCCModelImportedPart.h"
#include <FCGeometryInterface/FCGeoEnum.h>

namespace OCC {

FCOCCModelImportedPart::FCOCCModelImportedPart() : OCCShapeAgent(this)
{
    _shapeAgent = _occShapeAgent;
}

FC::FCGeoEnum::FCGeometryComType FCOCCModelImportedPart::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTImportPart;
}

bool FCOCCModelImportedPart::update()
{
    return true;
}

} // namespace OCC
