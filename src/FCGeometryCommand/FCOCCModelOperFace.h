/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelOperFace.h
 * @brief 面操作命令 OCC 实现（移植自 FITKOCCModelOperFace）
 */
#ifndef FCOCCMODELOPERFACE_H
#define FCOCCMODELOPERFACE_H

#include "FCAbstractOCCModel.h"
#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoModelOperFace.h>

namespace OCC {

class FCGEOMETRYCMD_API FCOCCModelOperFaceRemoveFace : public FC::FCGeoModelOperFaceRemoveFace, public OCCShapeAgent
{
public:
    FCOCCModelOperFaceRemoveFace() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelOperFaceRemoveFace() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelOperFaceFillGaps : public FC::FCGeoModelOperFace, public OCCShapeAgent
{
public:
    FCOCCModelOperFaceFillGaps() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelOperFaceFillGaps() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelOperFaceDeleteFloatingEdge : public FC::FCGeoModelOperFace, public OCCShapeAgent
{
public:
    FCOCCModelOperFaceDeleteFloatingEdge() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelOperFaceDeleteFloatingEdge() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

/** 填补面孔洞（OCC 实现，移植自 FITKOCCModelOperFaceFillHoles） */
class FCGEOMETRYCMD_API FCOCCModelOperFaceFillHoles : public FC::FCGeoModelOperFace, public OCCShapeAgent
{
public:
    FCOCCModelOperFaceFillHoles() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelOperFaceFillHoles() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

/** 延伸面（OCC 实现，移植自 FITKOCCModelExtendFace；当前为桩实现） */
class FCGEOMETRYCMD_API FCOCCModelExtendFace : public FC::FCGeoModelOperFaceExtendFace, public OCCShapeAgent
{
public:
    FCOCCModelExtendFace() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelExtendFace() override = default;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace OCC

#endif // FCOCCMODELOPERFACE_H
