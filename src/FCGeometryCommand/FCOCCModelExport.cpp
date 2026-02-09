/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

#include "FCOCCModelExport.h"
#include "FCAbstractOCCModel.h"
#include <FCGeometryInterface/FCAbsGeoCommand.h>
#include <FCData/FCDataRepo.h>
#include <BRepTools.hxx>
#include <STEPControl_Writer.hxx>
#include <IGESControl_Controller.hxx>
#include <IGESControl_Writer.hxx>
#include <StlAPI.hxx>

namespace OCC {

FC::FCGeoEnum::FCGeometryComType FCOCCModelExport::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTExport;
}

bool FCOCCModelExport::update()
{
    QString path = getFileName();
    if (path.isEmpty()) return false;
    QList<int> ids = getExportCommandIDs();
    if (ids.isEmpty()) return false;
    FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(ids.first());
    if (!cmd) return false;
    OCC::FCAbstractOCCModel* occModel = cmd->getTShapeAgent<OCC::FCAbstractOCCModel>();
    if (!occModel || !occModel->getShape() || occModel->getShape()->IsNull()) return false;
    return occModel->writeToFile(path);
}

} // namespace OCC
