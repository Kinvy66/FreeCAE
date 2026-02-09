/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

#include "FCOCCModelPart.h"

namespace OCC {

FCOCCModelPart::FCOCCModelPart()
    : OCCShapeAgent(this)
{
    _shapeAgent = _occShapeAgent;
}

FC::FCGeoEnum::FITKGeometryComType FCOCCModelPart::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTPart;
}

bool FCOCCModelPart::update()
{
    return false;
}

} // namespace OCC
