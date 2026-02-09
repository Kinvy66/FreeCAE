/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

#include "FCOCCModelSolid.h"

namespace OCC {

FCOCCModelClosedSurfaceSolid::FCOCCModelClosedSurfaceSolid() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FITKGeometryComType FCOCCModelClosedSurfaceSolid::getGeometryCommandType() { return FC::FCGeoEnum::FGTClosedSurfaceSolid; }
bool FCOCCModelClosedSurfaceSolid::update() { return false; }

FCOCCModelExtrudeSolid::FCOCCModelExtrudeSolid() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FITKGeometryComType FCOCCModelExtrudeSolid::getGeometryCommandType() { return FC::FCGeoEnum::FGTExtrudeSolid; }
bool FCOCCModelExtrudeSolid::update() { return false; }

FCOCCModelRevolSolid::FCOCCModelRevolSolid() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FITKGeometryComType FCOCCModelRevolSolid::getGeometryCommandType() { return FC::FCGeoEnum::FGTRevolSolid; }
bool FCOCCModelRevolSolid::update() { return false; }

FCOCCModelSweepSolid::FCOCCModelSweepSolid() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FITKGeometryComType FCOCCModelSweepSolid::getGeometryCommandType() { return FC::FCGeoEnum::FGTSweepSolid; }
bool FCOCCModelSweepSolid::update() { return false; }

FCOCCModelMultiSectionSolid::FCOCCModelMultiSectionSolid() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FITKGeometryComType FCOCCModelMultiSectionSolid::getGeometryCommandType() { return FC::FCGeoEnum::FGTMultiSectionSolid; }
bool FCOCCModelMultiSectionSolid::update() { return false; }

} // namespace OCC
