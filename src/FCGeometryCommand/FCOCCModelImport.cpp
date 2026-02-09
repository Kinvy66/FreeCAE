/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

#include "FCOCCModelImport.h"
#include <BRepTools.hxx>
#include <TopoDS_Shape.hxx>
#include <QFile>

namespace OCC {

FCOCCModelImport::FCOCCModelImport()
    : OCCShapeAgent(this)
{
    _shapeAgent = _occShapeAgent;
}

FC::FCGeoEnum::FITKGeometryComType FCOCCModelImport::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTImport;
}

bool FCOCCModelImport::update()
{
    QString path = getFileName();
    if (path.isEmpty()) return false;
    QByteArray ba = path.toUtf8();
    const char* cpath = ba.constData();
    TopoDS_Shape shape;
    if (path.endsWith(QStringLiteral("brep"), Qt::CaseInsensitive)) {
        if (!BRepTools::Read(shape, cpath)) return false;
    } else {
        return false;
    }
    if (shape.IsNull()) return false;
    _occShapeAgent->updateShape(shape);
    return true;
}

} // namespace OCC
