/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCStiffener.cpp
 * @brief 肋特征命令 OCC 实现（移植自 FITKGeoCompOCC FITKOCCFeature FITKOCCStiffener，轮廓为单边）
 */
#include "FCOCCStiffener.h"
#include "FCAbstractOCCModel.h"
#include <FCDataRepo.h>
#include <FCGeometryInterface/FCGeoEnum.h>
#include <FCModelInterface/FCModelEnum.h>

#include <gp_Lin.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
#include <BRep_Tool.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <Geom_Line.hxx>
#include <Geom_Plane.hxx>
#include <GeomLib_Tool.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <BRepFeat_MakeLinearForm.hxx>
#include <TopExp_Explorer.hxx>
#include <Precision.hxx>

#include <cmath>

namespace OCC {

static TopoDS_Shape getSolidFrom(const FC::VirtualShape& vs)
{
    if (vs.CmdId <= 0) return TopoDS_Shape();
    FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(vs.CmdId);
    if (!cmd) return TopoDS_Shape();
    FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
    if (!occ || !occ->getShape() || occ->getShape()->IsNull()) return TopoDS_Shape();
    return occ->getShape(FC::FCModelEnum::FMSSolid, vs.VirtualTopoId);
}

static TopoDS_Shape getEdgeFrom(const FC::VirtualShape& vs)
{
    if (vs.CmdId <= 0) return TopoDS_Shape();
    FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(vs.CmdId);
    if (!cmd) return TopoDS_Shape();
    FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
    if (!occ || !occ->getShape() || occ->getShape()->IsNull()) return TopoDS_Shape();
    return occ->getShape(FC::FCModelEnum::FMSEdge, vs.VirtualTopoId);
}

static bool getUByCrossPoint(TopoDS_Shape& shape, TopoDS_Shape& edge, double& u1, double& u2)
{
    double first, last;
    const TopoDS_Edge& edgeShape = TopoDS::Edge(edge);
    Handle(Geom_Curve) curve = BRep_Tool::Curve(edgeShape, first, last);
    Handle(Geom_Line) lineFin = Handle(Geom_Line)::DownCast(curve);
    if (lineFin.IsNull()) return false;

    gp_Lin lin = lineFin->Lin();
    gp_Pnt pt = lin.Location();
    gp_Dir dir = lin.Direction();
    TopoDS_Edge edgeInf = BRepBuilderAPI_MakeEdge(gp_Lin(pt, dir));

    try
    {
        TopoDS_Shape pointsEdgeInf = BRepAlgoAPI_Section(shape, edgeInf);
        double newFirst = 0., newLast = 0.;
        int nIntPtsInf = 0;
        for (TopExp_Explorer iter(pointsEdgeInf, TopAbs_VERTEX); iter.More(); iter.Next())
        {
            TopoDS_Vertex vertShape = TopoDS::Vertex(iter.Value());
            gp_Pnt crossPt = BRep_Tool::Pnt(vertShape);
            double pu;
            if (!GeomLib_Tool::Parameter(lineFin, crossPt, Precision::Confusion(), pu)) continue;
            if (nIntPtsInf) {
                newFirst = (std::min)(newFirst, pu);
                newLast = (std::max)(newLast, pu);
            } else {
                newFirst = pu;
                newLast = pu;
            }
            nIntPtsInf++;
        }
        if (nIntPtsInf == 0) return false;

        gp_Pnt startPt, endPt;
        lineFin->D0(newFirst + Precision::Confusion(), startPt);
        lineFin->D0(newLast - Precision::Confusion(), endPt);
        edge = BRepBuilderAPI_MakeEdge(startPt, endPt);
        u1 = newFirst;
        u2 = newLast;
    }
    catch (...) { return false; }
    return true;
}

FC::FCGeoEnum::FITKGeometryComType FCOCCStiffener::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTStiffener;
}

bool FCOCCStiffener::update()
{
    FC::VirtualShape srcVs = sourceShape();
    FC::VirtualShape prfVs = profileShape();
    if (srcVs.CmdId <= 0 || prfVs.CmdId <= 0) return false;

    TopoDS_Shape sourceTopo = getSolidFrom(srcVs);
    TopoDS_Shape profileTopo = getEdgeFrom(prfVs);
    if (sourceTopo.IsNull() || profileTopo.IsNull() || profileTopo.ShapeType() != TopAbs_EDGE) return false;

    double u1, u2;
    if (!getUByCrossPoint(sourceTopo, profileTopo, u1, u2)) return false;

    double first, last;
    Handle(Geom_Curve) curve = BRep_Tool::Curve(TopoDS::Edge(profileTopo), first, last);
    if (curve.IsNull()) return false;
    Standard_Real uMid = 0.5 * (first + last);
    gp_Pnt mid;
    gp_Vec lineVec;
    curve->D1(uMid, mid, lineVec);
    gp_Dir lineDir(lineVec);
    gp_Vec zVec(0, 0, 1);
    gp_Vec normVec = lineVec.Crossed(zVec);
    if (normVec.Magnitude() < 1e-6) normVec = gp_Vec(1, 0, 0);
    gp_Dir normal(normVec);

    Handle(Geom_Plane) aplane = new Geom_Plane(gp_Pnt(mid.X(), mid.Y(), mid.Z()), normal);
    gp_Vec vec1 = gp_Vec(normal) * thickness1();
    gp_Vec vec2 = gp_Vec(normal.Reversed()) * thickness2();
    TopoDS_Wire wire = BRepBuilderAPI_MakeWire(TopoDS::Edge(profileTopo));

    try
    {
        BRepFeat_MakeLinearForm aform(sourceTopo, wire, aplane, vec1, vec2, 1, Standard_True);
        aform.Perform();
        if (!aform.IsDone()) return false;
        _occShapeAgent->updateShape(aform.Shape());
        return true;
    }
    catch (...) { return false; }
}

} // namespace OCC
