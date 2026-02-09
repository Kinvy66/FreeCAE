/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelImportFlattenAssembly.cpp
 * @brief 扁平装配导入 OCC 实现（移植自 FITKGeoCompOCC FITKOCCModelImportFlattenAssembly，简化版：读入为单一形状）
 */
#include "FCOCCModelImportFlattenAssembly.h"
#include <FCGeometryInterface/FCGeoEnum.h>

#include <TopoDS_Shape.hxx>
#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
#include <StlAPI.hxx>
#include <STEPControl_Reader.hxx>
#include <IGESControl_Reader.hxx>

#include <QFile>

namespace OCC {

FC::FCGeoEnum::FCGeometryComType FCOCCModelImportFlattenAssembly::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTImportFlattenAssembly;
}

bool FCOCCModelImportFlattenAssembly::update()
{
    QString path = getFileName();
    if (path.isEmpty() || !QFile::exists(path)) return false;

    QString suffix = path.split(".").last().trimmed().toLower();
    QByteArray ba = path.toUtf8();
    const char* cpath = ba.constData();

    try
    {
        if (suffix == "brep")
        {
            TopoDS_Shape shape;
            BRep_Builder builder;
            if (!BRepTools::Read(shape, cpath, builder)) return false;
            _occShapeAgent->updateShape(shape);
            return true;
        }
        if (suffix == "stl")
        {
            TopoDS_Shape shape;
            if (!StlAPI::Read(shape, cpath)) return false;
            _occShapeAgent->updateShape(shape);
            return true;
        }
        if (suffix == "stp" || suffix == "step")
        {
            STEPControl_Reader reader;
            if (reader.ReadFile(cpath) != IFSelect_RetDone) return false;
            if (reader.TransferRoots() <= 0) return false;
            TopoDS_Shape shape = reader.OneShape();
            if (shape.IsNull()) return false;
            _occShapeAgent->updateShape(shape);
            return true;
        }
        if (suffix == "igs" || suffix == "iges")
        {
            IGESControl_Reader reader;
            if (reader.ReadFile(cpath) != IFSelect_RetDone) return false;
            if (reader.TransferRoots() <= 0) return false;
            TopoDS_Shape shape = reader.OneShape();
            if (shape.IsNull()) return false;
            _occShapeAgent->updateShape(shape);
            return true;
        }
    }
    catch (Standard_Failure const&)
    {
        return false;
    }
    catch (...)
    {
        return false;
    }
    return false;
}

} // namespace OCC
