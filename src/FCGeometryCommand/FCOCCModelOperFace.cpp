/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelOperFace.cpp
 * @brief 面操作命令 OCC 实现（移植自 FITKGeoCompOCC FITKOCCModelOperFace）
 * RemoveFace / FillGaps / DeleteFloatingEdge
 */
#include "FCOCCModelOperFace.h"
#include "FCAbstractOCCModel.h"
#include <FCGeometryInterface/FCGeoEnum.h>
#include <FCDataRepo.h>
#include <FCModelInterface/FCModelEnum.h>

#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Compound.hxx>
#include <TopExp_Explorer.hxx>
#include <TopExp.hxx>
#include <BRep_Builder.hxx>
#include <BRepTools_ReShape.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepOffsetAPI_Sewing.hxx>
#include <ShapeUpgrade_UnifySameDomain.hxx>
#include <TopTools_IndexedDataMapOfShapeListOfShape.hxx>

#include <QSet>

namespace OCC {

static TopoDS_Shape getShapeFromSource(const FC::VirtualShape& vs)
{
    if (vs.CmdId <= 0) return TopoDS_Shape();
    FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(vs.CmdId);
    if (!cmd) return TopoDS_Shape();
    FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
    if (!occ || !occ->getShape() || occ->getShape()->IsNull()) return TopoDS_Shape();
    return *occ->getShape();
}

static TopoDS_Shape getSubShape(const FC::VirtualShape& vs, FC::FCModelEnum::FITKModelSetType type)
{
    if (vs.CmdId <= 0) return TopoDS_Shape();
    FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(vs.CmdId);
    if (!cmd) return TopoDS_Shape();
    FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
    if (!occ || !occ->getShape() || occ->getShape()->IsNull()) return TopoDS_Shape();
    return occ->getShape(type, vs.VirtualTopoId);
}

// ----- RemoveFace -----
FC::FCGeoEnum::FITKGeometryComType FCOCCModelOperFaceRemoveFace::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTRemoveFace;
}

bool FCOCCModelOperFaceRemoveFace::update()
{
    QList<FC::VirtualShape> vshapes = getVShapes();
    if (vshapes.isEmpty()) return false;

    QList<TopoDS_Shape> fullShapes;
    QList<TopoDS_Face> faces;
    QSet<int> seenCmdIds;
    for (const FC::VirtualShape& vs : vshapes)
    {
        FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(vs.CmdId);
        if (!cmd) return false;
        FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
        if (!occ || !occ->getShape() || occ->getShape()->IsNull()) continue;
        TopoDS_Shape faceShape = occ->getShape(FC::FCModelEnum::FMSSurface, vs.VirtualTopoId);
        if (faceShape.IsNull() || faceShape.ShapeType() != TopAbs_FACE) continue;
        faces.append(TopoDS::Face(faceShape));
        if (!seenCmdIds.contains(vs.CmdId))
        {
            seenCmdIds.insert(vs.CmdId);
            fullShapes.append(*occ->getShape());
        }
    }
    if (faces.isEmpty() || fullShapes.isEmpty()) return false;

    Handle(BRepTools_ReShape) reShape = new BRepTools_ReShape;
    for (const TopoDS_Face& f : faces)
        reShape->Remove(f);

    BRep_Builder builder;
    TopoDS_Compound compound;
    builder.MakeCompound(compound);
    for (const TopoDS_Shape& fullShape : fullShapes)
    {
        TopoDS_Shape newShape = reShape->Apply(fullShape);
        if (!newShape.IsNull())
            builder.Add(compound, newShape);
    }
    _occShapeAgent->updateShape(compound);
    return true;
}

// ----- FillGaps -----
static TopoDS_Shape makeWireFromEdges(const QList<TopoDS_Shape>& edgeShapes, bool& ok)
{
    ok = false;
    BRepBuilderAPI_MakeWire mw;
    for (const TopoDS_Shape& s : edgeShapes)
    {
        if (s.IsNull() || s.ShapeType() != TopAbs_EDGE) return TopoDS_Shape();
        mw.Add(TopoDS::Edge(s));
    }
    mw.Build();
    if (!mw.IsDone()) return TopoDS_Shape();
    ok = true;
    return mw.Shape();
}

FC::FCGeoEnum::FITKGeometryComType FCOCCModelOperFaceFillGaps::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTFillGapsFace;
}

bool FCOCCModelOperFaceFillGaps::update()
{
    QList<FC::VirtualShape> vshapes = getVShapes();
    if (vshapes.isEmpty()) return false;

    QList<TopoDS_Shape> edgeShapes;
    for (const FC::VirtualShape& vs : vshapes)
    {
        TopoDS_Shape s = getSubShape(vs, FC::FCModelEnum::FMSEdge);
        if (s.IsNull()) return false;
        edgeShapes.append(s);
    }

    FC::FCAbsGeoCommand* firstCmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(vshapes.first().CmdId);
    if (!firstCmd) return false;
    FCAbstractOCCModel* firstOcc = firstCmd->getTShapeAgent<FCAbstractOCCModel>();
    if (!firstOcc || !firstOcc->getShape() || firstOcc->getShape()->IsNull()) return false;
    int nFaces = firstOcc->getShapeCount(FC::FCModelEnum::FMSSurface);
    if (nFaces < 1) return false;
    TopoDS_Shape mFaceShape = firstOcc->getShape(FC::FCModelEnum::FMSSurface, 1);
    if (mFaceShape.IsNull() || mFaceShape.ShapeType() != TopAbs_FACE) return false;
    TopoDS_Face mFace = TopoDS::Face(mFaceShape);

    bool wireOk = false;
    TopoDS_Shape wireShape = makeWireFromEdges(edgeShapes, wireOk);
    if (!wireOk || wireShape.IsNull() || wireShape.ShapeType() != TopAbs_WIRE) return false;
    TopoDS_Wire closedWire = TopoDS::Wire(wireShape);
    TopoDS_Face nFace = BRepBuilderAPI_MakeFace(closedWire);
    if (nFace.IsNull()) return false;

    BRepOffsetAPI_Sewing sewing;
    sewing.Add(mFace);
    sewing.Add(nFace);
    sewing.Perform();
    TopoDS_Shape sewedShape = sewing.SewedShape();
    ShapeUpgrade_UnifySameDomain unif(sewedShape, false, true, false);
    unif.Build();
    _occShapeAgent->updateShape(unif.Shape());
    return true;
}

// ----- DeleteFloatingEdge -----
FC::FCGeoEnum::FITKGeometryComType FCOCCModelOperFaceDeleteFloatingEdge::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTDeleteFloatingEdge;
}

bool FCOCCModelOperFaceDeleteFloatingEdge::update()
{
    QList<FC::VirtualShape> vshapes = getVShapes();
    if (vshapes.isEmpty()) return false;

    FC::VirtualShape vs0 = vshapes.first();
    if (vs0.CmdId <= 0) return false;
    FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(vs0.CmdId);
    if (!cmd) return false;
    FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
    if (!occ || !occ->getShape() || occ->getShape()->IsNull()) return false;
    TopoDS_Shape fullShape = *occ->getShape();

    TopTools_IndexedDataMapOfShapeListOfShape vToE;
    TopExp::MapShapesAndAncestors(fullShape, TopAbs_VERTEX, TopAbs_EDGE, vToE);
    BRep_Builder b;
    TopoDS_Compound toRemove;
    b.MakeCompound(toRemove);
    for (TopExp_Explorer exp(fullShape, TopAbs_EDGE); exp.More(); exp.Next())
    {
        TopoDS_Edge e = TopoDS::Edge(exp.Current());
        TopoDS_Vertex v1, v2;
        TopExp::Vertices(e, v1, v2);
        int n1 = vToE.Contains(v1) ? vToE.FindFromKey(v1).Extent() : 0;
        int n2 = vToE.Contains(v2) ? vToE.FindFromKey(v2).Extent() : 0;
        if (n1 == 1 || n2 == 1)
            b.Add(toRemove, e);
    }
    if (toRemove.IsNull()) return false;
    BRepTools_ReShape reShape;
    reShape.Remove(toRemove);
    TopoDS_Shape result = reShape.Apply(fullShape);
    if (result.IsNull()) return false;
    ShapeUpgrade_UnifySameDomain unif(result, true, true, false);
    unif.Build();
    _occShapeAgent->updateShape(unif.Shape());
    return true;
}

} // namespace OCC
