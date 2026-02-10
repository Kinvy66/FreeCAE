/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelSketch2D.h
 * @brief OCC 2D 草图命令（移植自 FITKGeoCompOCC FITKOCCModelSketch2D）
 * 汇总子命令的边为 Compound，供扫略/多截面等使用。
 */
#ifndef FCOCCMODELSKETCH2D_H
#define FCOCCMODELSKETCH2D_H

#include "FCAbstractOCCModel.h"
#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoSketch2D.h>
#include <QHash>

class TopoDS_Shape;

namespace OCC {

class FCGEOMETRYCMD_API FCOCCModelSketch2D : public FC::FCGeoSketch2D, public OCCShapeAgent
{
public:
    FCOCCModelSketch2D();
    ~FCOCCModelSketch2D() override;
    FC::FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;

    /** 收集所有子命令的边为 Compound（供 Sweep/MultiSection 等使用） */
    bool getShape(TopoDS_Shape& oShape);

protected:
    /** 子命令形状状态缓存，用于跳过未变化的 rebuild */
    QHash<int, int> m_cmdIdShapeHash;
};

} // namespace OCC

#endif // FCOCCMODELSKETCH2D_H
