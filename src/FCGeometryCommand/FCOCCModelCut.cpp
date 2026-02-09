/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

#include "FCOCCModelCut.h"
#include "FCAbstractOCCModel.h"
#include <FCData/FCDataRepo.h>
#include <FCModelInterface/FCModelEnum.h>
#include <gp_Vec.hxx>
#include <gp_Ax1.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepOffsetAPI_MakePipeShell.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>
#include <ShapeAnalysis.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Wire.hxx>
#include <Precision.hxx>

namespace OCC {

namespace {
TopoDS_Shape getSubShapeFromVirtualShape(const FC::VirtualShape& vs, FC::FCModelEnum::FITKModelSetType type)
{
    if (vs.isNull() || vs.CmdId <= 0) return TopoDS_Shape();
    FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(vs.CmdId);
    if (!cmd) return TopoDS_Shape();
    FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
    if (!occ || !occ->getShape() || occ->getShape()->IsNull()) return TopoDS_Shape();
    return occ->getShape(type, vs.VirtualTopoId);
}
TopoDS_Shape getSolidFromCmdId(int cmdId)
{
    if (cmdId <= 0) return TopoDS_Shape();
    FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(cmdId);
    if (!cmd) return TopoDS_Shape();
    FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
    if (!occ || !occ->getShape() || occ->getShape()->IsNull()) return TopoDS_Shape();
    return *occ->getShape();
}

TopoDS_Wire shapeToWire(const TopoDS_Shape& s)
{
    if (s.IsNull()) return TopoDS_Wire();
    if (s.ShapeType() == TopAbs_WIRE) return TopoDS::Wire(s);
    if (s.ShapeType() == TopAbs_EDGE) return BRepBuilderAPI_MakeWire(TopoDS::Edge(s)).Wire();
    if (s.ShapeType() == TopAbs_FACE) return ShapeAnalysis::OuterWire(TopoDS::Face(s));
    if (s.ShapeType() == TopAbs_COMPOUND) {
        BRepBuilderAPI_MakeWire mw;
        for (TopExp_Explorer exp(s, TopAbs_EDGE); exp.More(); exp.Next())
            mw.Add(TopoDS::Edge(exp.Current()));
        return mw.IsDone() ? TopoDS::Wire(mw.Shape()) : TopoDS_Wire();
    }
    return TopoDS_Wire();
}
} // namespace

FCOCCModelExtrudeCut::FCOCCModelExtrudeCut() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FITKGeometryComType FCOCCModelExtrudeCut::getGeometryCommandType() { return FC::FCGeoEnum::FGTExtrudeCut; }
bool FCOCCModelExtrudeCut::update()
{
    int baseCmdId = getInputCmdId();
    TopoDS_Shape baseShape = getSolidFromCmdId(baseCmdId);
    if (baseShape.IsNull()) return false;

    FC::VirtualShape src = sourceSurface();
    if (src.isNull()) return false;
    TopoDS_Shape faceShape = getSubShapeFromVirtualShape(src, FC::FCModelEnum::FMSSurface);
    if (faceShape.IsNull() || faceShape.ShapeType() != TopAbs_FACE) return false;
    TopoDS_Face face = TopoDS::Face(faceShape);

    std::array<double, 3> dir = direction();
    gp_Vec vec(dir[0], dir[1], dir[2]);
    double mag = vec.Magnitude();
    if (mag <= Precision::Confusion()) return false;
    if (extrudeType() != 1) return false;
    vec.Scale(length() / mag);

    try {
        TopoDS_Shape toolShape = BRepPrimAPI_MakePrism(face, vec).Shape();
        if (toolShape.IsNull()) return false;
        BRepAlgoAPI_Cut cut(baseShape, toolShape);
        if (!cut.IsDone()) return false;
        TopoDS_Shape result = cut.Shape();
        if (result.IsNull()) return false;
        _occShapeAgent->updateShape(result);
        return true;
    } catch (...) {
        return false;
    }
}

FCOCCModelRevolCut::FCOCCModelRevolCut() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FITKGeometryComType FCOCCModelRevolCut::getGeometryCommandType() { return FC::FCGeoEnum::FGTRevolCut; }
bool FCOCCModelRevolCut::update()
{
    int baseCmdId = getInputCmdId();
    TopoDS_Shape baseShape = getSolidFromCmdId(baseCmdId);
    if (baseShape.IsNull()) return false;

    FC::VirtualShape src = sourceSurface();
    if (src.isNull()) return false;
    TopoDS_Shape faceShape = getSubShapeFromVirtualShape(src, FC::FCModelEnum::FMSSurface);
    if (faceShape.IsNull() || faceShape.ShapeType() != TopAbs_FACE) return false;
    TopoDS_Face face = TopoDS::Face(faceShape);

    std::array<double, 3> p1 = rotateAxisPoint1(), p2 = rotateAxisPoint2();
    gp_Pnt origin(p1[0], p1[1], p1[2]);
    gp_Vec axisVec(p2[0] - p1[0], p2[1] - p1[1], p2[2] - p1[2]);
    if (axisVec.Magnitude() <= Precision::Confusion()) return false;
    gp_Ax1 axis(origin, gp_Dir(axisVec));
    double angleDeg = angle();
    if (angleDeg <= 0.0 || angleDeg >= 360.0) return false;
    double angleRad = angleDeg * 3.14159265358979323846 / 180.0;

    try {
        TopoDS_Shape toolShape = BRepPrimAPI_MakeRevol(face, axis, angleRad).Shape();
        if (toolShape.IsNull()) return false;
        BRepAlgoAPI_Cut cut(baseShape, toolShape);
        if (!cut.IsDone()) return false;
        TopoDS_Shape result = cut.Shape();
        if (result.IsNull()) return false;
        _occShapeAgent->updateShape(result);
        return true;
    } catch (...) {
        return false;
    }
}

FCOCCModelSweepCut::FCOCCModelSweepCut() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FITKGeometryComType FCOCCModelSweepCut::getGeometryCommandType() { return FC::FCGeoEnum::FGTSweepCut; }
bool FCOCCModelSweepCut::update()
{
    int baseCmdId = getInputCmdId();
    TopoDS_Shape baseShape = getSolidFromCmdId(baseCmdId);
    if (baseShape.IsNull()) return false;

    QList<FC::VirtualShape> pathCurves = curves();
    if (pathCurves.isEmpty()) return false;
    FC::VirtualShape prof = profile();
    if (prof.isNull()) return false;

    TopoDS_Shape pathShape;
    if (pathCurves.size() == 1) {
        pathShape = getSubShapeFromVirtualShape(pathCurves.first(), FC::FCModelEnum::FMSEdge);
        if (pathShape.IsNull()) return false;
    } else {
        BRepBuilderAPI_MakeWire mw;
        for (const FC::VirtualShape& vs : pathCurves) {
            TopoDS_Shape e = getSubShapeFromVirtualShape(vs, FC::FCModelEnum::FMSEdge);
            if (!e.IsNull() && e.ShapeType() == TopAbs_EDGE) mw.Add(TopoDS::Edge(e));
        }
        if (!mw.IsDone()) return false;
        pathShape = mw.Wire();
    }
    TopoDS_Wire pathWire = shapeToWire(pathShape);
    if (pathWire.IsNull()) return false;

    TopoDS_Shape profShape = getSubShapeFromVirtualShape(prof, FC::FCModelEnum::FMSSurface);
    if (profShape.IsNull()) profShape = getSubShapeFromVirtualShape(prof, FC::FCModelEnum::FMSEdge);
    TopoDS_Wire profWire = shapeToWire(profShape);
    if (profWire.IsNull()) return false;

    try {
        BRepOffsetAPI_MakePipeShell pipe(pathWire);
        pipe.Add(profWire);
        pipe.SetTransitionMode(BRepBuilderAPI_RightCorner);
        pipe.SetDiscreteMode();
        pipe.Build();
        if (!pipe.IsDone()) return false;
        if (!pipe.MakeSolid()) return false;
        TopoDS_Shape toolShape = pipe.Shape();
        if (toolShape.IsNull()) return false;
        BRepAlgoAPI_Cut cut(baseShape, toolShape);
        if (!cut.IsDone()) return false;
        TopoDS_Shape result = cut.Shape();
        if (result.IsNull()) return false;
        _occShapeAgent->updateShape(result);
        return true;
    } catch (...) {
        return false;
    }
}

FCOCCModelMultiSectionCut::FCOCCModelMultiSectionCut() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FITKGeometryComType FCOCCModelMultiSectionCut::getGeometryCommandType() { return FC::FCGeoEnum::FGTMultiSectionCut; }
bool FCOCCModelMultiSectionCut::update()
{
    int baseCmdId = getInputCmdId();
    TopoDS_Shape baseShape = getSolidFromCmdId(baseCmdId);
    if (baseShape.IsNull()) return false;

    QList<FC::VirtualShape> secList = sections();
    if (secList.size() < 2) return false;

    BRepOffsetAPI_ThruSections thruSection(true);
    for (const FC::VirtualShape& vs : secList) {
        if (vs.isNull()) return false;
        TopoDS_Shape s = getSubShapeFromVirtualShape(vs, FC::FCModelEnum::FMSSurface);
        if (s.IsNull()) s = getSubShapeFromVirtualShape(vs, FC::FCModelEnum::FMSEdge);
        if (s.IsNull()) return false;
        TopoDS_Wire w = shapeToWire(s);
        if (w.IsNull()) return false;
        thruSection.AddWire(w);
    }
    try {
        thruSection.Build();
        if (!thruSection.IsDone()) return false;
        TopoDS_Shape toolShape = thruSection.Shape();
        if (toolShape.IsNull()) return false;
        BRepAlgoAPI_Cut cut(baseShape, toolShape);
        if (!cut.IsDone()) return false;
        TopoDS_Shape result = cut.Shape();
        if (result.IsNull()) return false;
        _occShapeAgent->updateShape(result);
        return true;
    } catch (...) {
        return false;
    }
}

} // namespace OCC
