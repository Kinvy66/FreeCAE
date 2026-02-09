/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCNBodyUnite.h
 * @brief OCC 多实体布尔求和（移植自 FITKGeoCompOCC FITKOCCNBodyUnite）
 */
#ifndef FCOCCNBODYUNITE_H
#define FCOCCNBODYUNITE_H

#include "FCAbstractOCCModel.h"
#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoOperNBodyUnite.h>

namespace OCC {

class FCGEOMETRYCMD_API FCOCCNBodyUnite : public FC::FCGeoOperNBodyUnite, public OCCShapeAgent
{
public:
    explicit FCOCCNBodyUnite();
    ~FCOCCNBodyUnite() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace OCC

#endif // FCOCCNBODYUNITE_H
