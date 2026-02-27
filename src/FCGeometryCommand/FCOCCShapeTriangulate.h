/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCShapeTriangulate.h
 * @brief OCC 模型三角化（移植自 FITKGeoCompOCC FITKOCCShapeTriangulate）
 */
#ifndef FCOCCSHAPETRIANGULATE_H
#define FCOCCSHAPETRIANGULATE_H

#include "FCGeometryCommandAPI.h"
#include <FCData/FCType.h>

class TopoDS_Shape;

namespace OCC {

class FCAbstractOCCModel;

/**
 * @brief OCC 形状三角化（移植自 FITKGeoCompOCC FITKOCCShapeTriangulate）
 * 对形状做 BRepMesh 后，将顶点/边/面离散数据写入 FCGeometryMeshVS 供 VTK 显示。
 */
class FCGEOMETRYCMD_API FCOCCShapeTriangulate
{
public:
    explicit FCOCCShapeTriangulate(FCAbstractOCCModel* occModel);
    ~FCOCCShapeTriangulate() = default;

    void triangulate();

private:
    void triangulateShape(double factor = 0.001);
    void triangulate(const TopoDS_Shape& shape, double factor = 0.001);
    void discretePoint(FCID id, const TopoDS_Shape& shape);
    void discreteEdge(FCID id, const TopoDS_Shape& shape);
    void discreteFace(FCID id, const TopoDS_Shape& shape);

    FCAbstractOCCModel* _occModel{};
};

} // namespace OCC

#endif // FCOCCSHAPETRIANGULATE_H
