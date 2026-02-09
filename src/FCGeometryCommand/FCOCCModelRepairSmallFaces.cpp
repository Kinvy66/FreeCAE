/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelRepairSmallFaces.cpp
 * @brief 修复小面命令 OCC 实现（移植自 FITKOCCModelOperFace FITKOCCModelRepairSmallFaces，简化：无 FITKShapeFix，仅用 CheckSmallFace+ReShape 移除）
 */
#include "FCOCCModelRepairSmallFaces.h"
#include "FCAbstractOCCModel.h"
#include <FCDataRepo.h>
#include <FCGeometryInterface/FCGeoEnum.h>
#include <FCModelInterface/FCModelEnum.h>

#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Edge.hxx>
#include <TopExp_Explorer.hxx>
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>
#include <ShapeAnalysis_CheckSmallFace.hxx>
#include <BRepTools_ReShape.hxx>
#include <BRep_Builder.hxx>
#include <ShapeFix_Shape.hxx>

#include <QHash>
#include <QList>

namespace OCC {

static TopoDS_Shape getSubShape(const FC::VirtualShape& vs, FC::FCModelEnum::FITKModelSetType type)
{
    if (vs.CmdId <= 0) return TopoDS_Shape();
    FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(vs.CmdId);
    if (!cmd) return TopoDS_Shape();
    FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
    if (!occ || !occ->getShape() || occ->getShape()->IsNull()) return TopoDS_Shape();
    return occ->getShape(type, vs.VirtualTopoId);
}

FC::FCGeoEnum::FCGeometryComType FCOCCModelRepairSmallFaces::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTRepairSmallFaces;
}

bool FCOCCModelRepairSmallFaces::update()
{
    QList<FC::VirtualShape> vfaces = faces();
    if (vfaces.isEmpty()) return false;

    QHash<int, QList<TopoDS_Face>> cmdIdToFaces;
    QHash<int, TopoDS_Shape> cmdIdToFull;
    for (const FC::VirtualShape& vs : vfaces)
    {
        FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(vs.CmdId);
        if (!cmd) return false;
        FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
        if (!occ || !occ->getShape() || occ->getShape()->IsNull()) continue;
        TopoDS_Shape faceShape = getSubShape(vs, FC::FCModelEnum::FMSSurface);
        if (faceShape.IsNull() || faceShape.ShapeType() != TopAbs_FACE) continue;
        cmdIdToFaces[vs.CmdId].append(TopoDS::Face(faceShape));
        if (!cmdIdToFull.contains(vs.CmdId))
            cmdIdToFull[vs.CmdId] = *occ->getShape();
    }
    if (cmdIdToFull.isEmpty()) return false;

    ShapeAnalysis_CheckSmallFace checkSmall;
    BRep_Builder builder;
    TopoDS_Compound compound;
    builder.MakeCompound(compound);
    int nRemovedTotal = 0;

    for (auto it = cmdIdToFull.begin(); it != cmdIdToFull.end(); ++it)
    {
        int cid = it.key();
        TopoDS_Shape fullShape = it.value();
        const QList<TopoDS_Face>& faceList = cmdIdToFaces[cid];
        if (faceList.isEmpty()) { builder.Add(compound, fullShape); continue; }

        Bnd_Box box;
        BRepBndLib::Add(fullShape, box, false);
        double tol = sqrt(box.SquareExtent()) * 0.05;
        if (tol < 1e-6) tol = 1e-6;
        checkSmall.SetTolerance(tol);

        BRepTools_ReShape reShape;
        int nRemoved = 0;
        for (const TopoDS_Face& f : faceList)
        {
            if (f.IsNull()) continue;
            bool isSpot = checkSmall.CheckSpotFace(f, tol);
            TopoDS_Edge e1, e2;
            bool isStrip = checkSmall.CheckStripFace(f, e1, e2, tol);
            if (isSpot || isStrip)
            {
                reShape.Remove(f);
                nRemoved++;
            }
        }
        nRemovedTotal += nRemoved;
        TopoDS_Shape result = reShape.Apply(fullShape);
        if (result.IsNull()) result = fullShape;
        else
        {
            ShapeFix_Shape fix(result);
            fix.Perform();
            if (!fix.Shape().IsNull()) result = fix.Shape();
        }
        builder.Add(compound, result);
    }

    if (nRemovedTotal == 0) return false;
    _occShapeAgent->updateShape(compound);
    return true;
}

} // namespace OCC
