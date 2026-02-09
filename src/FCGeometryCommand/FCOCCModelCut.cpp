/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

#include "FCOCCModelCut.h"

namespace OCC {

FCOCCModelExtrudeCut::FCOCCModelExtrudeCut() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FITKGeometryComType FCOCCModelExtrudeCut::getGeometryCommandType() { return FC::FCGeoEnum::FGTExtrudeCut; }
bool FCOCCModelExtrudeCut::update() { return false; }

FCOCCModelRevolCut::FCOCCModelRevolCut() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FITKGeometryComType FCOCCModelRevolCut::getGeometryCommandType() { return FC::FCGeoEnum::FGTRevolCut; }
bool FCOCCModelRevolCut::update() { return false; }

FCOCCModelSweepCut::FCOCCModelSweepCut() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FITKGeometryComType FCOCCModelSweepCut::getGeometryCommandType() { return FC::FCGeoEnum::FGTSweepCut; }
bool FCOCCModelSweepCut::update() { return false; }

FCOCCModelMultiSectionCut::FCOCCModelMultiSectionCut() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FITKGeometryComType FCOCCModelMultiSectionCut::getGeometryCommandType() { return FC::FCGeoEnum::FGTMultiSectionCut; }
bool FCOCCModelMultiSectionCut::update() { return false; }

} // namespace OCC
