/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCVirtualTopoCreator.h
 * @brief OCC 虚拟拓扑创建器（移植自 FITKGeoCompOCC FITKOCCVirtualTopoCreator）
 */
#ifndef FCOCCVIRTUALTOPOCREATOR_H
#define FCOCCVIRTUALTOPOCREATOR_H

#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsVirtualTopo.h>

class TopoDS_Shape;

namespace FC { class FCVirtualTopoManager; }

namespace OCC {

/**
 * @brief OCC 几何形状虚拓扑包装（对应 FITKOCCTopoShape）
 */
class FCGEOMETRYCMD_API FCOCCTopoShape : public FC::FCAbsVirtualTopoShape
{
public:
    explicit FCOCCTopoShape(const TopoDS_Shape& shape);
    ~FCOCCTopoShape() override;
    bool isSameShape(void* shape) override;
    const TopoDS_Shape& getTopoShape() const;

private:
    TopoDS_Shape* _shape{};
};

/**
 * @brief 几何虚拓扑创建（对应 FITKOCCVirtualTopoCreator）
 */
class FCGEOMETRYCMD_API FCOCCVirtualTopoCreator
{
public:
    /**
     * @brief 创建 OCC 虚拓扑并填入管理器
     * @param shape 根形状
     * @param topoMgr 虚拓扑管理器（非空）
     */
    static void createOCCTopos(const TopoDS_Shape& shape, FC::FCVirtualTopoManager* topoMgr);
};

} // namespace OCC

#endif // FCOCCVIRTUALTOPOCREATOR_H
