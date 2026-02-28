/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

#include "FCOCCShapeTriangulate.h"
#include "FCAbstractOCCModel.h"
#include "FCOCCVirtualTopoCreator.h"
#include <FCGeometryInterface/FCVirtualTopoManager.h>
#include <FCGeometryInterface/FCGeometryMeshVS.h>
#include <FCGeometryInterface/FCGeometryMeshEntity.h>
#include <FCGeometryInterface/FCGeoEnum.h>

#include <TopoDS_Shape.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS.hxx>
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRep_Tool.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <GCPnts_UniformAbscissa.hxx>
#include <IMeshTools_Parameters.hxx>
#include <cmath>

namespace OCC {

FCOCCShapeTriangulate::FCOCCShapeTriangulate(FCAbstractOCCModel* occModel)
    : _occModel(occModel)
{}

void FCOCCShapeTriangulate::triangulate()
{
    if (!_occModel) return;
    FC::FCVirtualTopoManager* vtmanager = _occModel->getVirtualTopoManager();
    FC::FCGeometryMeshVS* meshVS = _occModel->getMeshVS();
    if (!meshVS || !vtmanager) return;

    TopoDS_Shape* shape = _occModel->getShape();
    if (!shape || shape->IsNull()) return;

    meshVS->clear();
    triangulate(*shape, 0.001);

    FC::FCShapeVirtualTopoManager* sm = vtmanager->getShapeVirtualTopoManager(FC::FCGeoEnum::VSPoint);
    if (sm) {
        for (int i = 0, n = sm->getDataCount(); i < n; ++i) {
            FC::FCAbsVirtualTopo* vtopo = sm->getDataByIndex(i);
            if (!vtopo) continue;
            FCOCCTopoShape* occShape = vtopo->getShapeT<FCOCCTopoShape>();
            if (!occShape) continue;
            const TopoDS_Shape& s = occShape->getTopoShape();
            if (s.IsNull()) continue;
            discretePoint(vtopo->getDataObjectID(), s);
        }
    }

    sm = vtmanager->getShapeVirtualTopoManager(FC::FCGeoEnum::VSEdge);
    if (sm) {
        for (int i = 0, n = sm->getDataCount(); i < n; ++i) {
            FC::FCAbsVirtualTopo* vtopo = sm->getDataByIndex(i);
            if (!vtopo) continue;
            FCOCCTopoShape* occShape = vtopo->getShapeT<FCOCCTopoShape>();
            if (!occShape) continue;
            const TopoDS_Shape& s = occShape->getTopoShape();
            if (s.IsNull()) continue;
            discreteEdge(vtopo->getDataObjectID(), s);
        }
    }

    sm = vtmanager->getShapeVirtualTopoManager(FC::FCGeoEnum::VSFace);
    if (sm) {
        for (int i = 0, n = sm->getDataCount(); i < n; ++i) {
            FC::FCAbsVirtualTopo* vtopo = sm->getDataByIndex(i);
            if (!vtopo) continue;
            FCOCCTopoShape* occShape = vtopo->getShapeT<FCOCCTopoShape>();
            if (!occShape) continue;
            const TopoDS_Shape& s = occShape->getTopoShape();
            if (s.IsNull()) continue;
            discreteFace(vtopo->getDataObjectID(), s);
        }
    }
}

void FCOCCShapeTriangulate::triangulate(const TopoDS_Shape& shape, double factor)
{
    if (shape.IsNull()) return;
    try {
        Bnd_Box box;
        BRepBndLib::Add(shape, box);
        double xMin, xMax, yMin, yMax, zMin, zMax;
        box.Get(xMin, yMin, zMin, xMax, yMax, zMax);
        double sizeFactor = std::sqrt(
            std::pow(xMax - xMin, 2) + std::pow(yMax - yMin, 2) + std::pow(zMax - zMin, 2));
        double deflection = factor * (sizeFactor < 1e-6 ? 1.0 : sizeFactor);
        IMeshTools_Parameters params;
        params.Deflection = deflection;
        BRepMesh_IncrementalMesh(const_cast<TopoDS_Shape&>(shape), params);
    } catch (...) {}
}

void FCOCCShapeTriangulate::triangulateShape(double factor)
{
    if (!_occModel) return;
    TopoDS_Shape* shape = _occModel->getShape();
    if (!shape || shape->IsNull()) return;
    triangulate(*shape, factor);
}

void FCOCCShapeTriangulate::discretePoint(FCID id, const TopoDS_Shape& shape)
{
    TopoDS_Vertex vertex = TopoDS::Vertex(shape);
    gp_Pnt pt = BRep_Tool::Pnt(vertex);
    FC::FCGeoMeshVSPt* vpt = new FC::FCGeoMeshVSPt();
    vpt->setXYZ(static_cast<float>(pt.X()), static_cast<float>(pt.Y()), static_cast<float>(pt.Z()));
    _occModel->getMeshVS()->insertPoint(id, vpt);
}

void FCOCCShapeTriangulate::discreteEdge(FCID id, const TopoDS_Shape& shape)
{
    const TopoDS_Edge& edge = TopoDS::Edge(shape);
    TopLoc_Location loc;
    const Handle(Poly_Polygon3D)& mesh = BRep_Tool::Polygon3D(edge, loc);
    FC::FCGeoMeshVSEdgeEntity* vsedge = new FC::FCGeoMeshVSEdgeEntity();

    if (!mesh.IsNull()) {
        int nPts = mesh->NbNodes();
        gp_Trsf trans(loc.Transformation());
        const TColgp_Array1OfPnt& nodes = mesh->Nodes();
        for (int i = 1; i <= nPts; ++i) {
            gp_Pnt pt = nodes.Value(i);
            pt.Transform(trans);
            FC::FCGeoMeshVSPt* vpt = new FC::FCGeoMeshVSPt();
            vpt->setXYZ(static_cast<float>(pt.X()), static_cast<float>(pt.Y()), static_cast<float>(pt.Z()));
            vsedge->appendPoint(vpt);
        }
    } else {
        // Polygon3D 为空：BRepMesh 仅对面生成 Triangulation，边的数据在 PolygonOnTriangulation 中。
        // 使用 BRepAdaptor_Curve + GCPnts 对边进行轻量级离散，避免逐边调用 BRepMesh。
        double first, last;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
        if (curve.IsNull() || std::fabs(last - first) < 1e-12) {
            delete vsedge;
            return;
        }
        const int nSample = 32;
        BRepAdaptor_Curve adaptor(edge);
        GCPnts_UniformAbscissa sampler(adaptor, nSample);
        if (!sampler.IsDone()) {
            delete vsedge;
            return;
        }
        gp_Trsf trans(edge.Location().Transformation());
        for (int i = 1; i <= sampler.NbPoints(); ++i) {
            gp_Pnt pt = adaptor.Value(sampler.Parameter(i));
            pt.Transform(trans);
            FC::FCGeoMeshVSPt* vpt = new FC::FCGeoMeshVSPt();
            vpt->setXYZ(static_cast<float>(pt.X()), static_cast<float>(pt.Y()), static_cast<float>(pt.Z()));
            vsedge->appendPoint(vpt);
        }
    }
    if (vsedge->getPointCount() < 2) {
        delete vsedge;
        return;
    }
    _occModel->getMeshVS()->insertEdge(id, vsedge);
}

void FCOCCShapeTriangulate::discreteFace(FCID id, const TopoDS_Shape& shape)
{
    const TopoDS_Face& face = TopoDS::Face(shape);
    TopLoc_Location loc;
    const Handle(Poly_Triangulation)& mesh = BRep_Tool::Triangulation(face, loc);
    if (mesh.IsNull()) return;

    int nPts = mesh->NbNodes();
    int nCells = mesh->NbTriangles();
    gp_Trsf trans(loc.Transformation());
    FC::FCGeoMeshVSFaceEntity* vsface = new FC::FCGeoMeshVSFaceEntity();

    // 使用 Node(i) / Triangle(i) 接口（OCC 7.6+ 及当前版本已移除 Nodes()/Triangles()）
    for (int i = 1; i <= nPts; ++i) {
        gp_Pnt pt = mesh->Node(i);
        pt.Transform(trans);
        FC::FCGeoMeshVSPt* vpt = new FC::FCGeoMeshVSPt();
        vpt->setXYZ(static_cast<float>(pt.X()), static_cast<float>(pt.Y()), static_cast<float>(pt.Z()));
        vsface->appendPoint(vpt);
    }
    for (int i = 1; i <= nCells; ++i) {
        const Poly_Triangle& tri = mesh->Triangle(i);
        int pt1, pt2, pt3;
        tri.Get(pt1, pt2, pt3);
        if (face.Orientation() == TopAbs_REVERSED)
            vsface->appendMeshTri(new FC::FCGeoMeshVSTri(pt3 - 1, pt2 - 1, pt1 - 1));
        else
            vsface->appendMeshTri(new FC::FCGeoMeshVSTri(pt1 - 1, pt2 - 1, pt3 - 1));
    }
    _occModel->getMeshVS()->insertFace(id, vsface);
}

} // namespace OCC
