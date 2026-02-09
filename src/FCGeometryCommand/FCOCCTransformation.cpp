/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

#include "FCOCCTransformation.h"
#include <FCData/FCDataRepo.h>
#include <gp_Trsf.hxx>
#include <gp_Vec.hxx>
#include <gp_Ax1.hxx>
#include <gp_Pnt.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <TopoDS_Shape.hxx>

namespace OCC {

static TopoDS_Shape getShapeFromSource(const FC::VirtualShape& vs)
{
    if (vs.CmdId <= 0) return TopoDS_Shape();
    FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(vs.CmdId);
    if (!cmd) return TopoDS_Shape();
    OCC::FCAbstractOCCModel* occModel = cmd->getTShapeAgent<OCC::FCAbstractOCCModel>();
    if (!occModel || !occModel->getShape() || occModel->getShape()->IsNull()) return TopoDS_Shape();
    return *occModel->getShape();
}

#define OCC_TRANSFORM_CTOR(Name) \
Name::Name() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }

OCC_TRANSFORM_CTOR(FCOCCModelTransform)
OCC_TRANSFORM_CTOR(FCOCCModelTransformByTwoPoints)
OCC_TRANSFORM_CTOR(FCOCCModelTransformByDirAndDis)
OCC_TRANSFORM_CTOR(FCOCCModelRotate)
OCC_TRANSFORM_CTOR(FCOCCModelScale)
OCC_TRANSFORM_CTOR(FCOCCModelMirror)

FC::FCGeoEnum::FITKGeometryComType FCOCCModelTransform::getGeometryCommandType() { return FC::FCGeoEnum::FGTTransform; }
FC::FCGeoEnum::FITKGeometryComType FCOCCModelTransformByTwoPoints::getGeometryCommandType() { return FC::FCGeoEnum::FGTTransformByTwoPoints; }
FC::FCGeoEnum::FITKGeometryComType FCOCCModelTransformByDirAndDis::getGeometryCommandType() { return FC::FCGeoEnum::FGTTransformByDirAndDis; }
FC::FCGeoEnum::FITKGeometryComType FCOCCModelRotate::getGeometryCommandType() { return FC::FCGeoEnum::FGTRotate; }
FC::FCGeoEnum::FITKGeometryComType FCOCCModelScale::getGeometryCommandType() { return FC::FCGeoEnum::FGTScale; }
FC::FCGeoEnum::FITKGeometryComType FCOCCModelMirror::getGeometryCommandType() { return FC::FCGeoEnum::FGTMirror; }

bool FCOCCModelTransform::update()
{
    TopoDS_Shape src = getShapeFromSource(sourceShape());
    if (src.IsNull()) return false;
    std::array<double, 3> v = vector();
    gp_Trsf trsf;
    trsf.SetTranslation(gp_Vec(v[0], v[1], v[2]));
    TopoDS_Shape result = BRepBuilderAPI_Transform(src, trsf, true).Shape();
    if (result.IsNull()) return false;
    _occShapeAgent->updateShape(result);
    return true;
}

bool FCOCCModelTransformByTwoPoints::update()
{
    TopoDS_Shape src = getShapeFromSource(sourceShape());
    if (src.IsNull()) return false;
    std::array<double, 3> p1 = startPoint(), p2 = endPoint();
    gp_Vec vec(p2[0] - p1[0], p2[1] - p1[1], p2[2] - p1[2]);
    gp_Trsf trsf;
    trsf.SetTranslation(vec);
    TopoDS_Shape result = BRepBuilderAPI_Transform(src, trsf, true).Shape();
    if (result.IsNull()) return false;
    _occShapeAgent->updateShape(result);
    return true;
}

bool FCOCCModelTransformByDirAndDis::update()
{
    TopoDS_Shape src = getShapeFromSource(sourceShape());
    if (src.IsNull()) return false;
    std::array<double, 3> d = direction();
    double len = distance();
    gp_Vec vec(d[0] * len, d[1] * len, d[2] * len);
    gp_Trsf trsf;
    trsf.SetTranslation(vec);
    TopoDS_Shape result = BRepBuilderAPI_Transform(src, trsf, true).Shape();
    if (result.IsNull()) return false;
    _occShapeAgent->updateShape(result);
    return true;
}

bool FCOCCModelRotate::update()
{
    TopoDS_Shape src = getShapeFromSource(sourceShape());
    if (src.IsNull()) return false;
    std::array<double, 3> p1 = axisStartPoint(), p2 = axisEndPoint();
    gp_Pnt origin(p1[0], p1[1], p1[2]);
    gp_Dir axisDir(p2[0] - p1[0], p2[1] - p1[1], p2[2] - p1[2]);
    gp_Ax1 axis(origin, axisDir);
    gp_Trsf trsf;
    trsf.SetRotation(axis, radian());
    TopoDS_Shape result = BRepBuilderAPI_Transform(src, trsf, true).Shape();
    if (result.IsNull()) return false;
    _occShapeAgent->updateShape(result);
    return true;
}

bool FCOCCModelScale::update()
{
    TopoDS_Shape src = getShapeFromSource(sourceShape());
    if (src.IsNull()) return false;
    std::array<double, 3> fac = factors(), base = basePoint();
    gp_Pnt p(base[0], base[1], base[2]);
    gp_Trsf trsf;
    trsf.SetScale(p, fac[0]);
    TopoDS_Shape result = BRepBuilderAPI_Transform(src, trsf, true).Shape();
    if (result.IsNull()) return false;
    _occShapeAgent->updateShape(result);
    return true;
}

bool FCOCCModelMirror::update()
{
    TopoDS_Shape src = getShapeFromSource(sourceShape());
    if (src.IsNull()) return false;
    FC::VirtualShape ref = reference();
    if (ref.isNull()) return false;
    return false;
}

} // namespace OCC
