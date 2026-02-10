/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

#include "FCOCCShapeTriangulate.h"
#include "FCAbstractOCCModel.h"

#include <TopoDS_Shape.hxx>
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <IMeshTools_Parameters.hxx>
#include <cmath>

namespace OCC {

FCOCCShapeTriangulate::FCOCCShapeTriangulate(FCAbstractOCCModel* occModel)
    : _occModel(occModel)
{}

void FCOCCShapeTriangulate::triangulate()
{
    if (!_occModel) return;
    TopoDS_Shape* shape = _occModel->getShape();
    if (!shape || shape->IsNull()) return;
    triangulateShape(0.001);
}

void FCOCCShapeTriangulate::triangulateShape(double factor)
{
    if (!_occModel) return;
    TopoDS_Shape* shape = _occModel->getShape();
    if (!shape || shape->IsNull()) return;
    try
    {
        Bnd_Box box;
        BRepBndLib::Add(*shape, box);
        double xMin, xMax, yMin, yMax, zMin, zMax;
        box.Get(xMin, yMin, zMin, xMax, yMax, zMax);
        double sizeFactor = std::sqrt(
            std::pow(xMax - xMin, 2) + std::pow(yMax - yMin, 2) + std::pow(zMax - zMin, 2));
        double deflection = factor * (sizeFactor < 1e-6 ? 1.0 : sizeFactor);
        IMeshTools_Parameters params;
        params.Deflection = deflection;
        BRepMesh_IncrementalMesh(*shape, params);
    }
    catch (...) { }
}

} // namespace OCC
