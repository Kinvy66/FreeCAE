/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

#include "FCOCCTransformation.h"
#include "FCAbstractOCCModel.h"
#include <FCData/FCDataRepo.h>
#include <FCModelInterface/FCModelEnum.h>
#include <FCGeometryInterface/FCAbsGeoDatum.h>
#include <gp_Trsf.hxx>
#include <gp_Vec.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS.hxx>
#include <TopExp_Explorer.hxx>
#include <BRep_Tool.hxx>
#include <Geom_Curve.hxx>
#include <Geom_Line.hxx>
#include <Geom_Plane.hxx>
#include <Geom_Surface.hxx>
#include <BRepBuilderAPI_Copy.hxx>
#include <BRep_Builder.hxx>
#include <TopoDS_Compound.hxx>
#include <Standard_Type.hxx>
#include <cmath>

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

static TopoDS_Edge getEdgeFromVirtualShape(const FC::VirtualShape& vs)
{
    if (vs.isNull() || vs.CmdId <= 0) return TopoDS_Edge();
    FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(vs.CmdId);
    if (!cmd) return TopoDS_Edge();
    FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
    if (!occ) return TopoDS_Edge();
    TopoDS_Shape s = occ->getShape(FC::FCModelEnum::FMSEdge, vs.VirtualTopoId);
    if (s.IsNull() || s.ShapeType() != TopAbs_EDGE) return TopoDS_Edge();
    return TopoDS::Edge(s);
}

#define OCC_TRANSFORM_CTOR(Name) \
Name::Name() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }

OCC_TRANSFORM_CTOR(FCOCCModelTransform)
OCC_TRANSFORM_CTOR(FCOCCModelTransformByTwoPoints)
OCC_TRANSFORM_CTOR(FCOCCModelTransformByDirAndDis)
OCC_TRANSFORM_CTOR(FCOCCModelRotate)
OCC_TRANSFORM_CTOR(FCOCCModelScale)
OCC_TRANSFORM_CTOR(FCOCCModelMirror)
OCC_TRANSFORM_CTOR(FCOCCModelRectangularPattern)
OCC_TRANSFORM_CTOR(FCOCCModelCircularPattern)

FC::FCGeoEnum::FCGeometryComType FCOCCModelTransform::getGeometryCommandType() { return FC::FCGeoEnum::FGTTransform; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelTransformByTwoPoints::getGeometryCommandType() { return FC::FCGeoEnum::FGTTransformByTwoPoints; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelTransformByDirAndDis::getGeometryCommandType() { return FC::FCGeoEnum::FGTTransformByDirAndDis; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelRotate::getGeometryCommandType() { return FC::FCGeoEnum::FGTRotate; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelScale::getGeometryCommandType() { return FC::FCGeoEnum::FGTScale; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelMirror::getGeometryCommandType() { return FC::FCGeoEnum::FGTMirror; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelRectangularPattern::getGeometryCommandType() { return FC::FCGeoEnum::FGTRectangularPattern; }
FC::FCGeoEnum::FCGeometryComType FCOCCModelCircularPattern::getGeometryCommandType() { return FC::FCGeoEnum::FGTCircularPattern; }

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
    if (ref.isNull() || ref.CmdId <= 0) return false;

    FC::FCAbsGeoCommand* refCmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(ref.CmdId);
    if (!refCmd) return false;

    gp_Trsf trans;
    FC::FCAbsGeoDatumPlane* datumPlane = dynamic_cast<FC::FCAbsGeoDatumPlane*>(refCmd);
    if (datumPlane) {
        double pos[3], nor[3];
        datumPlane->getPosition(pos);
        datumPlane->getNormal(nor);
        gp_Ax2 ax2(gp_Pnt(pos[0], pos[1], pos[2]), gp_Dir(nor[0], nor[1], nor[2]));
        trans.SetMirror(ax2);
    } else {
        OCC::FCAbstractOCCModel* refAgent = refCmd->getTShapeAgent<OCC::FCAbstractOCCModel>();
        if (!refAgent || !refAgent->getShape() || refAgent->getShape()->IsNull()) return false;
        TopoDS_Shape refShape = *refAgent->getShape();

        if (refShape.ShapeType() == TopAbs_SOLID || refShape.ShapeType() == TopAbs_SHELL || refShape.ShapeType() == TopAbs_COMPSOLID) {
            TopExp_Explorer exp(refShape, TopAbs_FACE);
            if (!exp.More()) return false;
            refShape = exp.Current();
        }
        switch (refShape.ShapeType()) {
        case TopAbs_VERTEX: {
            TopoDS_Vertex vertex = TopoDS::Vertex(refShape);
            if (vertex.IsNull()) return false;
            trans.SetMirror(BRep_Tool::Pnt(vertex));
            break;
        }
        case TopAbs_EDGE: {
            TopoDS_Edge edge = TopoDS::Edge(refShape);
            if (edge.IsNull()) return false;
            Standard_Real first, last;
            Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
            gp_Pnt start = curve->Value(first), end = curve->Value(last);
            gp_Dir axisDir(end.X() - start.X(), end.Y() - start.Y(), end.Z() - start.Z());
            trans.SetMirror(gp_Ax1(start, axisDir));
            break;
        }
        case TopAbs_FACE: {
            TopoDS_Face face = TopoDS::Face(refShape);
            if (face.IsNull()) return false;
            Handle(Geom_Surface) surface = BRep_Tool::Surface(face);
            if (surface->DynamicType() != STANDARD_TYPE(Geom_Plane)) return false;
            Handle(Geom_Plane) plane = Handle(Geom_Plane)::DownCast(surface);
            if (plane.IsNull()) return false;
            const gp_Ax3& ax3 = plane->Position();
            trans.SetMirror(gp_Ax2(ax3.Location(), ax3.Direction()));
            break;
        }
        default:
            return false;
        }
    }

    TopoDS_Shape result = BRepBuilderAPI_Transform(src, trans, true).Shape();
    if (result.IsNull()) return false;
    _occShapeAgent->updateShape(result);
    return true;
}

bool FCOCCModelRectangularPattern::update()
{
    FC::VirtualShape src = sourceShape();
    if (src.isNull()) return false;
    TopoDS_Shape baseShape = getShapeFromSource(src);
    if (baseShape.IsNull()) return false;

    int n1 = firstCount(), n2 = secondCount();
    double space1 = firstSpacing(), space2 = secondSpacing();
    if (firstDefinition() == FC::FCGeoModelPattern::InstanceAndTotal && firstTotal() > 0 && n1 > 0)
        space1 = firstTotal() / n1;
    else if (firstDefinition() == FC::FCGeoModelPattern::SpacingAndTotal && firstSpacing() > 0 && firstTotal() > 0)
        n1 = static_cast<int>(std::floor(firstTotal() / firstSpacing()));
    if (secondDefinition() == FC::FCGeoModelPattern::InstanceAndTotal && secondTotal() > 0 && n2 > 0)
        space2 = secondTotal() / n2;
    else if (secondDefinition() == FC::FCGeoModelPattern::SpacingAndTotal && secondSpacing() > 0 && secondTotal() > 0)
        n2 = static_cast<int>(std::floor(secondTotal() / secondSpacing()));
    if (n1 <= 0 || n2 <= 0 || space1 <= 0 || space2 <= 0) return false;

    std::array<double, 3> d1 = firstDirection(), d2 = secondDirection();
    gp_Vec vec1(d1[0], d1[1], d1[2]), vec2(d2[0], d2[1], d2[2]);
    if (vec1.Magnitude() < 1e-10 || vec2.Magnitude() < 1e-10) return false;
    double mag1 = vec1.Magnitude(), mag2 = vec2.Magnitude();
    gp_Vec v1(vec1.X() / mag1 * space1, vec1.Y() / mag1 * space1, vec1.Z() / mag1 * space1);
    gp_Vec v2(vec2.X() / mag2 * space2, vec2.Y() / mag2 * space2, vec2.Z() / mag2 * space2);

    BRep_Builder builder;
    TopoDS_Compound compound;
    builder.MakeCompound(compound);
    try {
        for (int i2 = 0; i2 < n2; ++i2) {
            for (int i1 = 0; i1 < n1; ++i1) {
                gp_Trsf tr;
                tr.SetTranslation(v1 * i1 + v2 * i2);
                TopoDS_Shape copy = BRepBuilderAPI_Copy(baseShape).Shape();
                copy.Move(tr);
                builder.Add(compound, copy);
            }
        }
        _occShapeAgent->updateShape(compound);
        return true;
    } catch (...) {
        return false;
    }
}

bool FCOCCModelCircularPattern::update()
{
    FC::VirtualShape src = sourceShape();
    FC::VirtualShape axisVs = axis();
    if (src.isNull() || axisVs.isNull()) return false;
    TopoDS_Shape baseShape = getShapeFromSource(src);
    if (baseShape.IsNull()) return false;
    TopoDS_Edge axisEdge = getEdgeFromVirtualShape(axisVs);
    if (axisEdge.IsNull()) return false;

    Standard_Real first, last;
    Handle(Geom_Curve) curve = BRep_Tool::Curve(axisEdge, first, last);
    if (curve.IsNull() || curve->DynamicType() != STANDARD_TYPE(Geom_Line)) return false;
    gp_Pnt start = curve->Value(first), end = curve->Value(last);
    gp_Dir axisDir(end.XYZ() - start.XYZ());
    if (isReverse()) axisDir.Reverse();
    gp_Ax1 rotateAxis(start, axisDir);

    int nAxis = axisCount();
    double spacingDeg = axisSpacing();
    if (firstDefinition() == FC::FCGeoModelPattern::InstanceAndTotal && axisTotal() > 0 && nAxis > 0)
        spacingDeg = axisTotal() / nAxis;
    else if (firstDefinition() == FC::FCGeoModelPattern::SpacingAndTotal && axisSpacing() > 0 && axisTotal() > 0)
        nAxis = static_cast<int>(std::floor(axisTotal() / axisSpacing()));
    if (nAxis <= 0 || spacingDeg <= 0) return false;

    const double radPerDeg = 3.14159265358979323846 / 180.0;
    BRep_Builder builder;
    TopoDS_Compound compound;
    builder.MakeCompound(compound);
    try {
        for (int i = 0; i < nAxis; ++i) {
            gp_Trsf tr;
            tr.SetRotation(rotateAxis, spacingDeg * radPerDeg * i);
            TopoDS_Shape copy = BRepBuilderAPI_Copy(baseShape).Shape();
            copy.Move(tr);
            builder.Add(compound, copy);
        }
        _occShapeAgent->updateShape(compound);
        return true;
    } catch (...) {
        return false;
    }
}

} // namespace OCC
