/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelFromMesh.cpp
 * @brief 从网格生成几何命令 OCC 实现（移植自 FITKGeoCompOCC FITKOCCModelFromMesh）
 */
#include "FCOCCModelFromMesh.h"
#include <FCGeometryInterface/FCGeoEnum.h>

#include <TopoDS_Shape.hxx>
#include <StlAPI.hxx>

#include <QDir>
#include <QFile>
#include <QTextStream>

namespace OCC {

static bool writeSTL(const QString& path,
    const QHash<int, FC::FCGeoModelFromMesh::MeshNodeCoordinates>& nodes,
    const QList<FC::FCGeoModelFromMesh::MeshTri>& tris)
{
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) return false;
    QTextStream out(&f);
    out.setRealNumberPrecision(8);
    out << "solid mesh\n";
    for (const FC::FCGeoModelFromMesh::MeshTri& t : tris)
    {
        auto it1 = nodes.constFind(t.node1);
        auto it2 = nodes.constFind(t.node2);
        auto it3 = nodes.constFind(t.node3);
        if (it1 == nodes.constEnd() || it2 == nodes.constEnd() || it3 == nodes.constEnd()) continue;
        const FC::FCGeoModelFromMesh::MeshNodeCoordinates* n1 = &(*it1);
        const FC::FCGeoModelFromMesh::MeshNodeCoordinates* n2 = &(*it2);
        const FC::FCGeoModelFromMesh::MeshNodeCoordinates* n3 = &(*it3);
        out << "  facet normal " << t.normal[0] << " " << t.normal[1] << " " << t.normal[2] << "\n";
        out << "    outer loop\n";
        out << "      vertex " << n1->x << " " << n1->y << " " << n1->z << "\n";
        out << "      vertex " << n2->x << " " << n2->y << " " << n2->z << "\n";
        out << "      vertex " << n3->x << " " << n3->y << " " << n3->z << "\n";
        out << "    endloop\n";
        out << "  endfacet\n";
    }
    out << "endsolid mesh\n";
    f.close();
    return true;
}

FC::FCGeoEnum::FCGeometryComType FCOCCModelFromMesh::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTModelFromMesh;
}

bool FCOCCModelFromMesh::update()
{
    const auto& nodes = getMeshNodeCoordinates();
    const auto& tris = getMeshTriangles();
    if (nodes.isEmpty() || tris.isEmpty()) return false;

    QString path = QDir::tempPath() + "/FreeCAE_geom_mesh.stl";
    if (!writeSTL(path, nodes, tris)) return false;

    QByteArray ba = path.toUtf8();
    TopoDS_Shape shape;
    bool ok = StlAPI::Read(shape, ba.constData());
    if (!ok) return false;
    _occShapeAgent->updateShape(shape);
    return true;
}

} // namespace OCC
