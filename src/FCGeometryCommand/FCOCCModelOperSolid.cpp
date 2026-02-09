/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

#include "FCOCCModelOperSolid.h"
#include "FCAbstractOCCModel.h"
#include <FCData/FCDataRepo.h>
#include <FCModelInterface/FCModelEnum.h>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS.hxx>
#include <BRepFilletAPI_MakeChamfer.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepAlgoAPI_Defeaturing.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepBndLib.hxx>
#include <Bnd_Box.hxx>
#include <TopoDS_Face.hxx>
#include <TopTools_ListOfShape.hxx>
#include <TopExp_Explorer.hxx>
#include <Geom_Curve.hxx>
#include <Geom_Line.hxx>
#include <Geom_Plane.hxx>
#include <Geom_Surface.hxx>
#include <BRep_Tool.hxx>
#include <GeomAPI_ProjectPointOnSurf.hxx>
#include <GeomAPI_ProjectPointOnCurve.hxx>
#include <GeomAPI_ExtremaCurveCurve.hxx>
#include <gp_Trsf.hxx>
#include <gp_Ax2.hxx>
#include <cmath>

namespace OCC {

namespace {
TopoDS_Shape getSolidFromCmdId(int cmdId)
{
    if (cmdId <= 0) return TopoDS_Shape();
    FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(cmdId);
    if (!cmd) return TopoDS_Shape();
    FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
    if (!occ || !occ->getShape() || occ->getShape()->IsNull()) return TopoDS_Shape();
    return *occ->getShape();
}
TopoDS_Edge getEdgeFromVirtualShape(FC::FCAbsGeoCommand* cmd, const FC::VirtualShape& vs)
{
    if (!cmd || vs.isNull() || vs.VirtualTopoId <= 0) return TopoDS_Edge();
    FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
    if (!occ || !occ->getShape() || occ->getShape()->IsNull()) return TopoDS_Edge();
    TopoDS_Shape s = occ->getShape(FC::FCModelEnum::FMSEdge, vs.VirtualTopoId);
    if (s.IsNull() || s.ShapeType() != TopAbs_EDGE) return TopoDS_Edge();
    return TopoDS::Edge(s);
}

bool defeatureShape(int solidCmdId, const QList<FC::VirtualShape>& faceTopos, TopoDS_Shape& outputShape)
{
    FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(solidCmdId);
    if (!cmd) return false;
    FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
    if (!occ || !occ->getShape() || occ->getShape()->IsNull()) return false;
    const TopoDS_Shape& baseShape = *occ->getShape();

    TopTools_ListOfShape facesToRemove;
    for (const FC::VirtualShape& vs : faceTopos) {
        if (vs.CmdId != solidCmdId) continue;
        TopoDS_Shape s = occ->getShape(FC::FCModelEnum::FMSSurface, vs.VirtualTopoId);
        if (s.IsNull() || s.ShapeType() != TopAbs_FACE) return false;
        facesToRemove.Append(TopoDS::Face(s));
    }
    if (facesToRemove.IsEmpty()) return false;

    try {
        BRepAlgoAPI_Defeaturing def;
        def.SetShape(baseShape);
        for (TopTools_ListIteratorOfListOfShape it(facesToRemove); it.More(); it.Next())
            def.AddFaceToRemove(it.Value());
        def.Build();
        if (!def.IsDone()) return false;
        outputShape = def.Shape();
        return !outputShape.IsNull();
    } catch (...) {
        return false;
    }
}
} // namespace

FCOCCModelChamferSolid::FCOCCModelChamferSolid() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FITKGeometryComType FCOCCModelChamferSolid::getGeometryCommandType() { return FC::FCGeoEnum::FGTChamferSolid; }
bool FCOCCModelChamferSolid::update()
{
    int solidCmdId = getInputCmdId();
    TopoDS_Shape baseShape = getSolidFromCmdId(solidCmdId);
    if (baseShape.IsNull()) return false;
    double dist = getDistance();
    if (dist <= 0.0) return false;

    FC::FCAbsGeoCommand* solidCmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(solidCmdId);
    if (!solidCmd) return false;

    try {
        BRepFilletAPI_MakeChamfer mkChamfer(baseShape);
        for (const FC::VirtualShape& vs : getEdgeTopos()) {
            if (vs.CmdId != solidCmdId) continue;
            TopoDS_Edge edge = getEdgeFromVirtualShape(solidCmd, vs);
            if (!edge.IsNull())
                mkChamfer.Add(dist, edge);
        }
        mkChamfer.Build();
        if (!mkChamfer.IsDone()) return false;
        TopoDS_Shape result = mkChamfer.Shape();
        if (result.IsNull()) return false;
        _occShapeAgent->updateShape(result);
        return true;
    } catch (...) {
        return false;
    }
}

FCOCCModelFilletSolid::FCOCCModelFilletSolid() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FITKGeometryComType FCOCCModelFilletSolid::getGeometryCommandType() { return FC::FCGeoEnum::FGTFilletSolid; }
bool FCOCCModelFilletSolid::update()
{
    int solidCmdId = getInputCmdId();
    TopoDS_Shape baseShape = getSolidFromCmdId(solidCmdId);
    if (baseShape.IsNull()) return false;
    double radius = getRadius();
    if (radius <= 0.0) return false;

    FC::FCAbsGeoCommand* solidCmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(solidCmdId);
    if (!solidCmd) return false;

    try {
        BRepFilletAPI_MakeFillet mkFillet(baseShape);
        for (const FC::VirtualShape& vs : getEdgeTopos()) {
            if (vs.CmdId != solidCmdId) continue;
            TopoDS_Edge edge = getEdgeFromVirtualShape(solidCmd, vs);
            if (!edge.IsNull())
                mkFillet.Add(radius, edge);
        }
        mkFillet.Build();
        if (!mkFillet.IsDone()) return false;
        TopoDS_Shape result = mkFillet.Shape();
        if (result.IsNull()) return false;
        _occShapeAgent->updateShape(result);
        return true;
    } catch (...) {
        return false;
    }
}

FCOCCModelDefeatureSolid::FCOCCModelDefeatureSolid() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FITKGeometryComType FCOCCModelDefeatureSolid::getGeometryCommandType() { return FC::FCGeoEnum::FGTDefeature; }
bool FCOCCModelDefeatureSolid::update()
{
    int solidCmdId = getInputCmdId();
    if (solidCmdId <= 0) return false;
    TopoDS_Shape output;
    if (!defeatureShape(solidCmdId, getFaceTopos(), output)) return false;
    _occShapeAgent->updateShape(output);
    return true;
}

FCOCCModelRemoveChamferSolid::FCOCCModelRemoveChamferSolid() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FITKGeometryComType FCOCCModelRemoveChamferSolid::getGeometryCommandType() { return FC::FCGeoEnum::FGTRemoveChamfer; }
bool FCOCCModelRemoveChamferSolid::update()
{
    int solidCmdId = getInputCmdId();
    if (solidCmdId <= 0) return false;
    TopoDS_Shape output;
    if (!defeatureShape(solidCmdId, getFaceTopos(), output)) return false;
    _occShapeAgent->updateShape(output);
    return true;
}

FCOCCModelRemoveFilletSolid::FCOCCModelRemoveFilletSolid() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FITKGeometryComType FCOCCModelRemoveFilletSolid::getGeometryCommandType() { return FC::FCGeoEnum::FGTRemoveFillet; }
bool FCOCCModelRemoveFilletSolid::update()
{
    int solidCmdId = getInputCmdId();
    if (solidCmdId <= 0) return false;
    TopoDS_Shape output;
    if (!defeatureShape(solidCmdId, getFaceTopos(), output)) return false;
    _occShapeAgent->updateShape(output);
    return true;
}

FCOCCModelFillHoleSolid::FCOCCModelFillHoleSolid() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FITKGeometryComType FCOCCModelFillHoleSolid::getGeometryCommandType() { return FC::FCGeoEnum::FGTFillHole; }
bool FCOCCModelFillHoleSolid::update()
{
    int solidCmdId = getInputCmdId();
    if (solidCmdId <= 0) return false;
    TopoDS_Shape output;
    if (!defeatureShape(solidCmdId, getFaceTopos(), output)) return false;
    _occShapeAgent->updateShape(output);
    return true;
}

FCOCCModelCircularHole::FCOCCModelCircularHole() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
FC::FCGeoEnum::FITKGeometryComType FCOCCModelCircularHole::getGeometryCommandType() { return FC::FCGeoEnum::FGTCircularHole; }
bool FCOCCModelCircularHole::update()
{
    int baseCmdId = getInputCmdId();
    TopoDS_Shape baseShape = getSolidFromCmdId(baseCmdId);
    if (baseShape.IsNull()) return false;

    FC::FCAbsGeoCommand* faceCmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(getOperFace().CmdId);
    FC::FCAbsGeoCommand* edge1Cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(getOperEdge1().CmdId);
    FC::FCAbsGeoCommand* edge2Cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(getOperEdge2().CmdId);
    if (!faceCmd || !edge1Cmd || !edge2Cmd) return false;
    FCAbstractOCCModel* faceOcc = faceCmd->getTShapeAgent<FCAbstractOCCModel>();
    FCAbstractOCCModel* edge1Occ = edge1Cmd->getTShapeAgent<FCAbstractOCCModel>();
    FCAbstractOCCModel* edge2Occ = edge2Cmd->getTShapeAgent<FCAbstractOCCModel>();
    if (!faceOcc || !edge1Occ || !edge2Occ) return false;

    TopoDS_Shape fShape = faceOcc->getShape(FC::FCModelEnum::FMSSurface, getOperFace().VirtualTopoId);
    TopoDS_Shape e1Shape = edge1Occ->getShape(FC::FCModelEnum::FMSEdge, getOperEdge1().VirtualTopoId);
    TopoDS_Shape e2Shape = edge2Occ->getShape(FC::FCModelEnum::FMSEdge, getOperEdge2().VirtualTopoId);
    if (fShape.IsNull() || fShape.ShapeType() != TopAbs_FACE || e1Shape.IsNull() || e1Shape.ShapeType() != TopAbs_EDGE || e2Shape.IsNull() || e2Shape.ShapeType() != TopAbs_EDGE)
        return false;

    TopoDS_Face topoFace = TopoDS::Face(fShape);
    TopoDS_Edge topoEdge1 = TopoDS::Edge(e1Shape);
    TopoDS_Edge topoEdge2 = TopoDS::Edge(e2Shape);

    Standard_Real t1, t2;
    Handle(Geom_Curve) cur1 = BRep_Tool::Curve(topoEdge1, t1, t2);
    Handle(Geom_Curve) cur2 = BRep_Tool::Curve(topoEdge2, t1, t2);
    Handle(Geom_Surface) surf = BRep_Tool::Surface(topoFace);
    Handle(Geom_Line) line1 = Handle(Geom_Line)::DownCast(cur1);
    Handle(Geom_Line) line2 = Handle(Geom_Line)::DownCast(cur2);
    Handle(Geom_Plane) plane = Handle(Geom_Plane)::DownCast(surf);
    if (line1.IsNull() || line2.IsNull() || plane.IsNull()) return false;

    Bnd_Box bnd;
    for (TopExp_Explorer exp(baseShape, TopAbs_VERTEX); exp.More(); exp.Next())
        BRepBndLib::Add(exp.Current(), bnd);
    if (bnd.IsVoid()) return false;
    double xmin, ymin, zmin, xmax, ymax, zmax;
    bnd.Get(xmin, ymin, zmin, xmax, ymax, zmax);
    gp_Pnt cent((xmin + xmax) * 0.5, (ymin + ymax) * 0.5, (zmin + zmax) * 0.5);
    double depth = sqrt((xmax - xmin) * (xmax - xmin) + (ymax - ymin) * (ymax - ymin) + (zmax - zmin) * (zmax - zmin));
    if (depth < 1e-7) return false;

    GeomAPI_ProjectPointOnSurf projSurf(cent, plane);
    if (projSurf.NbPoints() == 0) return false;
    gp_Pnt ctOnPln = projSurf.Point(1);

    GeomAPI_ProjectPointOnCurve projCur1(ctOnPln, line1);
    GeomAPI_ProjectPointOnCurve projCur2(ctOnPln, line2);
    if (projCur1.NbPoints() == 0 || projCur2.NbPoints() == 0) return false;
    gp_Pnt ctOnCur1 = projCur1.Point(1), ctOnCur2 = projCur2.Point(1);

    const gp_Dir& surfNor = plane->Pln().Position().Direction();
    gp_Trsf tr;
    tr.SetRotation(gp_Ax1(gp_Pnt(), surfNor), 3.14159265358979 * 0.5);
    gp_Dir curNPDir1 = line1->Lin().Position().Direction().Transformed(tr);
    gp_Dir curNPDir2 = line2->Lin().Position().Direction().Transformed(tr);
    gp_Dir curCtDir1(ctOnPln.X() - ctOnCur1.X(), ctOnPln.Y() - ctOnCur1.Y(), ctOnPln.Z() - ctOnCur1.Z());
    gp_Dir curCtDir2(ctOnPln.X() - ctOnCur2.X(), ctOnPln.Y() - ctOnCur2.Y(), ctOnPln.Z() - ctOnCur2.Z());
    if (!curNPDir1.IsEqual(curCtDir1, 1e-6)) curNPDir1.Reverse();
    if (!curNPDir2.IsEqual(curCtDir2, 1e-6)) curNPDir2.Reverse();

    gp_Trsf tr1, tr2;
    tr1.SetTranslation(gp_Vec(curNPDir1.X() * getDistanceLocateHole1(), curNPDir1.Y() * getDistanceLocateHole1(), curNPDir1.Z() * getDistanceLocateHole1()));
    tr2.SetTranslation(gp_Vec(curNPDir2.X() * getDistanceLocateHole2(), curNPDir2.Y() * getDistanceLocateHole2(), curNPDir2.Z() * getDistanceLocateHole2()));
    Handle(Geom_Curve) transCur1 = Handle(Geom_Curve)::DownCast(line1->Transformed(tr1));
    Handle(Geom_Curve) transCur2 = Handle(Geom_Curve)::DownCast(line2->Transformed(tr2));
    if (transCur1.IsNull() || transCur2.IsNull()) return false;

    GeomAPI_ExtremaCurveCurve extCC(transCur1, transCur2);
    if (extCC.NbExtrema() == 0) return false;
    gp_Pnt pt1, pt2;
    extCC.Points(1, pt1, pt2);
    pt1.SetX(pt1.X() + surfNor.X() * depth * 0.1);
    pt1.SetY(pt1.Y() + surfNor.Y() * depth * 0.1);
    pt1.SetZ(pt1.Z() + surfNor.Z() * depth * 0.1);

    gp_Ax2 cylAxis(pt1, getFlip() ? surfNor : gp_Dir(-surfNor.X(), -surfNor.Y(), -surfNor.Z()));
    TopoDS_Shape cylinderShape = BRepPrimAPI_MakeCylinder(cylAxis, getDiameter() * 0.5, depth * 1.2);
    if (cylinderShape.IsNull()) return false;

    try {
        BRepAlgoAPI_Cut cut(baseShape, cylinderShape);
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
