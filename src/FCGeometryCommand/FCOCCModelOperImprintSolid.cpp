/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelOperImprintSolid.cpp
 * @brief 压印体命令 OCC 实现（移植自 FITKGeoCompOCC FITKOCCModelOperImprintSolid）
 */
#include "FCOCCModelOperImprintSolid.h"
#include "FCAbstractOCCModel.h"
#include <FCDataRepo.h>
#include <FCModelInterface/FCModelEnum.h>

#include <TopoDS_Shape.hxx>
#include <BRepAlgoAPI_Splitter.hxx>
#include <TopTools_ListOfShape.hxx>

namespace OCC {

static TopoDS_Shape getSolidFromSource(const FC::VirtualShape& vs)
{
    if (vs.CmdId <= 0) return TopoDS_Shape();
    FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(vs.CmdId);
    if (!cmd) return TopoDS_Shape();
    FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
    if (!occ || !occ->getShape() || occ->getShape()->IsNull()) return TopoDS_Shape();
    TopoDS_Shape s = occ->getShape(FC::FCModelEnum::FMSSolid, vs.VirtualTopoId);
    return s;
}

FC::FCGeoEnum::FITKGeometryComType FCOCCModelOperImprintSolid::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTImprintSolid;
}

bool FCOCCModelOperImprintSolid::update()
{
    QList<FC::VirtualShape> vshapes = getVShapes();
    if (vshapes.size() < 2) return false;

    TopTools_ListOfShape args, tools;
    for (const FC::VirtualShape& vs : vshapes)
    {
        TopoDS_Shape s = getSolidFromSource(vs);
        if (s.IsNull()) return false;
        args.Append(s);
        tools.Append(s);
    }
    BRepAlgoAPI_Splitter spli;
    spli.SetArguments(args);
    spli.SetTools(tools);
    spli.Build();
    if (spli.HasErrors()) return false;
    _occShapeAgent->updateShape(spli.Shape());
    return true;
}

} // namespace OCC
