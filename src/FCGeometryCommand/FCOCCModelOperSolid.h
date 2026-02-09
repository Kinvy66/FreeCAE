/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelOperSolid.h
 * @brief OCC 体特征操作（倒角/倒圆/移除/填孔/圆孔等）（移植自 FITKGeoCompOCC FITKOCCModelOperSolid）
 */
#ifndef FCOCCMODELOPERSOLID_H
#define FCOCCMODELOPERSOLID_H

#include "FCAbstractOCCModel.h"
#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoModelOperSolid.h>

namespace OCC {

class FCGEOMETRYCMD_API FCOCCModelChamferSolid : public FC::FCAbsGeoModelChamferSolid, public OCCShapeAgent
{
public:
    FCOCCModelChamferSolid();
    ~FCOCCModelChamferSolid() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelFilletSolid : public FC::FCAbsGeoModelFilletSolid, public OCCShapeAgent
{
public:
    FCOCCModelFilletSolid();
    ~FCOCCModelFilletSolid() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelDefeatureSolid : public FC::FCAbsGeoModelDefeatureSolid, public OCCShapeAgent
{
public:
    FCOCCModelDefeatureSolid();
    ~FCOCCModelDefeatureSolid() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelRemoveChamferSolid : public FC::FCAbsGeoModelRemoveChamferSolid, public OCCShapeAgent
{
public:
    FCOCCModelRemoveChamferSolid();
    ~FCOCCModelRemoveChamferSolid() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelRemoveFilletSolid : public FC::FCAbsGeoModelRemoveFilletSolid, public OCCShapeAgent
{
public:
    FCOCCModelRemoveFilletSolid();
    ~FCOCCModelRemoveFilletSolid() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelFillHoleSolid : public FC::FCAbsGeoModelFillHoleSolid, public OCCShapeAgent
{
public:
    FCOCCModelFillHoleSolid();
    ~FCOCCModelFillHoleSolid() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelCircularHole : public FC::FCAbsGeoModelCircularHoleSolid, public OCCShapeAgent
{
public:
    FCOCCModelCircularHole();
    ~FCOCCModelCircularHole() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace OCC

#endif // FCOCCMODELOPERSOLID_H
