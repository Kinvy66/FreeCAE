/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelCut.h
 * @brief OCC 切削功能（与 FreeCAE FCGeoModelCut 接口关联）
 */
#ifndef FCOCCMODELCUT_H
#define FCOCCMODELCUT_H

#include "FCAbstractOCCModel.h"
#include "FCAbsGeoModelCut.h"

namespace OCC
{
    class FCGEOMETRYCMD_API FCOCCModelExtrudeCut : public FC::FCAbsGeoModelExtrudeCut, public OCCShapeAgent
    {
    public:
        FCOCCModelExtrudeCut();
        ~FCOCCModelExtrudeCut() override = default;
        bool update() override;
    };

    class FCGEOMETRYCMD_API FCOCCModelRevolCut : public FC::FCAbsGeoModelRevolCut, public OCCShapeAgent
    {
    public:
        FCOCCModelRevolCut();
        ~FCOCCModelRevolCut() override = default;
        bool update() override;
    };

    class FCGEOMETRYCMD_API FCOCCModelSweepCut : public FC::FCAbsGeoModelSweepCut, public OCCShapeAgent
    {
    public:
        FCOCCModelSweepCut();
        ~FCOCCModelSweepCut() override = default;
        bool update() override;
    };

    class FCGEOMETRYCMD_API FCOCCModelMultiSectionCut : public FC::FCAbsGeoModelMultiSectionCut, public OCCShapeAgent
    {
    public:
        FCOCCModelMultiSectionCut();
        ~FCOCCModelMultiSectionCut() override = default;
        bool update() override;
    };
}

#endif
