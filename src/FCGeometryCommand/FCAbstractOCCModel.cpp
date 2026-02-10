/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCAbstractOCCModel.cpp
 * @brief OCC 模型基类实现（移植自 FITKGeoCompOCC FITKAbstractOCCModel）
 */
#include "FCAbstractOCCModel.h"
#include "FCOCCVirtualTopoCreator.h"
#include "FCOCCShapeTriangulate.h"
#include <FCGeometryInterface/FCVirtualTopoManager.h>
#include <FCGeometryInterface/FCGeometryMeshVS.h>

#include <TopoDS_Shape.hxx>
#include <TopExp_Explorer.hxx>
#include <TopExp.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <BRepTools.hxx>
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>
#include <STEPControl_Writer.hxx>
#include <IGESControl_Controller.hxx>
#include <IGESControl_Writer.hxx>
#include <StlAPI.hxx>

#include <QHash>
#include <limits>
#include <functional>

namespace OCC {

static QHash<FC::FCModelEnum::FITKModelSetType, TopAbs_ShapeEnum> s_occShapeEnums = {
    { FC::FCModelEnum::FMSPoint,   TopAbs_VERTEX },
    { FC::FCModelEnum::FMSEdge,    TopAbs_EDGE },
    { FC::FCModelEnum::FMSSurface, TopAbs_FACE },
    { FC::FCModelEnum::FMSSolid,   TopAbs_SOLID },
};

FCAbstractOCCModel::FCAbstractOCCModel(FC::FCAbsGeoCommand* command)
    : FC::FCAbsGeoShapeAgent(command)
{
    _shape = new TopoDS_Shape();
    _meshVS = new FC::FCGeometryMeshVS();
}

FCAbstractOCCModel::~FCAbstractOCCModel()
{
    if (_shape) { delete _shape; _shape = nullptr; }
}

int FCAbstractOCCModel::getShapeDim(const TopoDS_Shape& shape)
{
    if (shape.IsNull()) return -1;
    TopExp_Explorer exp(shape, TopAbs_SOLID);
    if (exp.More()) return 3;
    exp.Init(shape, TopAbs_FACE);
    if (exp.More()) return 2;
    exp.Init(shape, TopAbs_EDGE);
    if (exp.More()) return 1;
    exp.Init(shape, TopAbs_VERTEX);
    if (exp.More()) return 0;
    return -1;
}

TopoDS_Shape* FCAbstractOCCModel::getShape()
{
    return _shape;
}

bool FCAbstractOCCModel::writeToFile(const QString& file)
{
    if (!_shape || _shape->IsNull()) return false;
    QString lower = file.toLower();
    QByteArray ba = lower.toUtf8();
    const char* path = ba.constData();
    if (lower.endsWith(QStringLiteral("brep")))
        return BRepTools::Write(*_shape, path);
    if (lower.endsWith(QStringLiteral("stp")) || lower.endsWith(QStringLiteral("step"))) {
        STEPControl_Writer writer;
        writer.Transfer(*_shape, STEPControl_AsIs);
        return writer.Write(path);
    }
    if (lower.endsWith(QStringLiteral("igs")) || lower.endsWith(QStringLiteral("iges"))) {
        IGESControl_Controller::Init();
        IGESControl_Writer writer;
        writer.AddShape(*_shape);
        return writer.Write(path);
    }
    if (lower.endsWith(QStringLiteral("stl")))
        return StlAPI::Write(*_shape, path);
    return false;
}

int FCAbstractOCCModel::getShapeCount(FC::FCModelEnum::FITKModelSetType type)
{
    if (!s_occShapeEnums.contains(type)) return -1;
    TopTools_IndexedMapOfShape map;
    TopExp::MapShapes(*_shape, s_occShapeEnums.value(type), map);
    return map.Extent();
}

const TopoDS_Shape FCAbstractOCCModel::getShape(FC::FCModelEnum::FITKModelSetType type, int id)
{
    if (!s_occShapeEnums.contains(type)) return TopoDS_Shape();
    TopTools_IndexedMapOfShape map;
    TopExp::MapShapes(*_shape, s_occShapeEnums.value(type), map);
    return map.FindKey(id);
}

FC::FCGeoEnum::FCGeoEngine FCAbstractOCCModel::getGeoEngine()
{
    return FC::FCGeoEnum::FGEOCC;
}

FC::FCModelEnum::AbsModelType FCAbstractOCCModel::getAbsModelType()
{
    return FC::FCModelEnum::AMTGeometry;
}

bool FCAbstractOCCModel::getBoundaryBox(double* minPt, double* maxPt)
{
    if (!minPt || !maxPt || !_shape || _shape->IsNull()) return false;
    try {
        Bnd_Box box;
        BRepBndLib::Add(*_shape, box, false);
        gp_Pnt pmin = box.CornerMin();
        gp_Pnt pmax = box.CornerMax();
        minPt[0] = pmin.X(); minPt[1] = pmin.Y(); minPt[2] = pmin.Z();
        maxPt[0] = pmax.X(); maxPt[1] = pmax.Y(); maxPt[2] = pmax.Z();
        return true;
    } catch (...) {
        return false;
    }
}

static int shapeHashCode(const TopoDS_Shape& s)
{
    if (s.IsNull()) return 0;
    return static_cast<int>(std::hash<TopoDS_Shape>{}(s) & 0x7FFFFFFF);
}

void FCAbstractOCCModel::updateShape(const TopoDS_Shape& shape, bool buildVTopo)
{
    if (_shape && !_shape->IsNull() && !shape.IsNull() && shape.IsSame(*_shape))
        return;
    *_shape = shape;
    _hashCode = shapeHashCode(shape);
    if (_buildingTopo) return;
    if (_vtmanager && buildVTopo) _vtmanager->clear();
    if (_meshVS && buildVTopo) _meshVS->clear();
    if (_shape->IsNull()) return;
    if (buildVTopo) {
        buildVirtualTopo(false);
        triangulation();
    }
}

void FCAbstractOCCModel::buildVirtualTopo(bool keepTopos)
{
    Q_UNUSED(keepTopos);
    if (_buildingTopo || !_vtmanager || !_shape || _shape->IsNull()) return;
    _buildingTopo = true;
    FCOCCVirtualTopoCreator::createOCCTopos(*_shape, _vtmanager);
    _buildingTopo = false;
}

void FCAbstractOCCModel::triangulation()
{
    FCOCCShapeTriangulate tri(this);
    tri.triangulate();
}

int FCAbstractOCCModel::getDim()
{
    return getShapeDim(*_shape);
}

bool FCAbstractOCCModel::createShapeState(QVariant& stateVal)
{
    if (!_shape || _shape->IsNull()) return false;
    stateVal = shapeHashCode(*_shape);
    return true;
}

bool FCAbstractOCCModel::isSameState(QVariant& stateVal1, QVariant& stateVal2)
{
    return stateVal1.toInt() == stateVal2.toInt();
}

OCCShapeAgent::OCCShapeAgent(FC::FCAbsGeoCommand* c)
{
    _occShapeAgent = new FCAbstractOCCModel(c);
}

} // namespace OCC
