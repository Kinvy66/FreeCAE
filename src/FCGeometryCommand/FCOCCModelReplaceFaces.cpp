/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelReplaceFaces.cpp
 * @brief 替换面命令 OCC 实现（移植自 FITKGeoCompOCC FITKOCCModelOperFace FITKOCCModelReplaceFaces，简化：无 Part，单命令多面）
 */
#include "FCOCCModelReplaceFaces.h"
#include "FCAbstractOCCModel.h"
#include <FCDataRepo.h>
#include <FCGeometryInterface/FCGeoEnum.h>
#include <FCModelInterface/FCModelEnum.h>

#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shell.hxx>
#include <TopExp_Explorer.hxx>
#include <TopExp.hxx>
#include <TopTools_IndexedDataMapOfShapeListOfShape.hxx>
#include <BRep_Builder.hxx>
#include <BRepTools_ReShape.hxx>
#include <BRepFill_Filling.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <BRepBuilderAPI_Sewing.hxx>

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

FC::FCGeoEnum::FITKGeometryComType FCOCCModelReplaceFaces::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTReplaceFaces;
}

bool FCOCCModelReplaceFaces::update()
{
    QList<FC::VirtualShape> faceShapes = faces();
    if (faceShapes.isEmpty()) return false;

    int cmdId = faceShapes.first().CmdId;
    FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(cmdId);
    if (!cmd) return false;
    FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
    if (!occ || !occ->getShape() || occ->getShape()->IsNull()) return false;
    TopoDS_Shape fullShape = *occ->getShape();

    QList<TopoDS_Face> selectedFaces;
    for (const FC::VirtualShape& vs : faceShapes)
    {
        if (vs.CmdId != cmdId) return false;
        TopoDS_Shape s = getSubShape(vs, FC::FCModelEnum::FMSSurface);
        if (s.IsNull() || s.ShapeType() != TopAbs_FACE) return false;
        selectedFaces.append(TopoDS::Face(s));
    }

    TopTools_IndexedDataMapOfShapeListOfShape edgeToFaces;
    TopExp::MapShapesAndAncestors(fullShape, TopAbs_EDGE, TopAbs_FACE, edgeToFaces);

    QList<TopoDS_Edge> boundaryEdges;
    for (int i = 1; i <= edgeToFaces.Extent(); i++)
    {
        const TopoDS_Shape& edgeShape = edgeToFaces.FindKey(i);
        const TopTools_ListOfShape& faceList = edgeToFaces.FindFromIndex(i);
        int count = 0;
        for (TopTools_ListIteratorOfListOfShape it(faceList); it.More(); it.Next())
        {
            const TopoDS_Shape& f = it.Value();
            for (const TopoDS_Face& sel : selectedFaces)
                if (f.IsSame(sel)) { count++; break; }
        }
        if (count == 1)
            boundaryEdges.append(TopoDS::Edge(edgeShape));
    }
    if (boundaryEdges.isEmpty()) return false;

    BRepFill_Filling fill;
    for (const TopoDS_Edge& e : boundaryEdges)
        fill.Add(e, GeomAbs_C0, true);
    fill.Build();
    if (!fill.IsDone()) return false;
    TopoDS_Face newFace = fill.Face();
    if (newFace.IsNull()) return false;

    BRepTools_ReShape reShape;
    for (const TopoDS_Face& f : selectedFaces)
        reShape.Remove(f);
    TopoDS_Shape removed = reShape.Apply(fullShape);

    BRepBuilderAPI_Sewing sewer;
    sewer.Add(newFace);
    for (TopExp_Explorer exp(removed, TopAbs_FACE); exp.More(); exp.Next())
        sewer.Add(exp.Current());
    sewer.Perform();
    TopoDS_Shape sewed = sewer.SewedShape();
    if (sewed.IsNull()) return false;

    TopoDS_Shape result = sewed;
    int nShells = 0;
    TopoDS_Shell singleShell;
    for (TopExp_Explorer exp(sewed, TopAbs_SHELL); exp.More(); exp.Next(), nShells++)
        singleShell = TopoDS::Shell(exp.Current());
    if (nShells == 1 && !singleShell.IsNull())
    {
        TopoDS_Solid solid = BRepBuilderAPI_MakeSolid(singleShell);
        if (!solid.IsNull())
            result = solid;
    }

    _occShapeAgent->updateShape(result);
    return true;
}

} // namespace OCC
