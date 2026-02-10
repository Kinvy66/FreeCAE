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

namespace OCC {

class FCAbstractOCCModel;

/**
 * @brief OCC 形状三角化
 */
class FCGEOMETRYCMD_API FCOCCShapeTriangulate
{
public:
    explicit FCOCCShapeTriangulate(FCAbstractOCCModel* occModel);
    ~FCOCCShapeTriangulate() = default;

    void triangulate();

private:
    void triangulateShape(double factor = 0.001);

    FCAbstractOCCModel* _occModel{};
};

} // namespace OCC

#endif // FCOCCSHAPETRIANGULATE_H
