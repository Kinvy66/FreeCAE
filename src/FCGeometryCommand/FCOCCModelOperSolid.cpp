/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

#include "FCOCCModelOperSolid.h"

namespace OCC {

FCOCCModelChamferSolid::FCOCCModelChamferSolid() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FITKGeometryComType FCOCCModelChamferSolid::getGeometryCommandType() { return FC::FCGeoEnum::FGTChamferSolid; }
bool FCOCCModelChamferSolid::update() { return false; }

FCOCCModelFilletSolid::FCOCCModelFilletSolid() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FITKGeometryComType FCOCCModelFilletSolid::getGeometryCommandType() { return FC::FCGeoEnum::FGTFilletSolid; }
bool FCOCCModelFilletSolid::update() { return false; }

FCOCCModelDefeatureSolid::FCOCCModelDefeatureSolid() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FITKGeometryComType FCOCCModelDefeatureSolid::getGeometryCommandType() { return FC::FCGeoEnum::FGTDefeature; }
bool FCOCCModelDefeatureSolid::update() { return false; }

FCOCCModelRemoveChamferSolid::FCOCCModelRemoveChamferSolid() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FITKGeometryComType FCOCCModelRemoveChamferSolid::getGeometryCommandType() { return FC::FCGeoEnum::FGTRemoveChamfer; }
bool FCOCCModelRemoveChamferSolid::update() { return false; }

FCOCCModelRemoveFilletSolid::FCOCCModelRemoveFilletSolid() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FITKGeometryComType FCOCCModelRemoveFilletSolid::getGeometryCommandType() { return FC::FCGeoEnum::FGTRemoveFillet; }
bool FCOCCModelRemoveFilletSolid::update() { return false; }

FCOCCModelFillHoleSolid::FCOCCModelFillHoleSolid() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FITKGeometryComType FCOCCModelFillHoleSolid::getGeometryCommandType() { return FC::FCGeoEnum::FGTFillHole; }
bool FCOCCModelFillHoleSolid::update() { return false; }

FCOCCModelCircularHole::FCOCCModelCircularHole() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FITKGeometryComType FCOCCModelCircularHole::getGeometryCommandType() { return FC::FCGeoEnum::FGTCircularHole; }
bool FCOCCModelCircularHole::update() { return false; }

} // namespace OCC
