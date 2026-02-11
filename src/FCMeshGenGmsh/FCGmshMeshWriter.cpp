/**
 * @file FCGmshMeshWriter.cpp
 * @brief 网格写出实现（使用 FCUnstructuredMeshVTK::writeToFile 写 VTK，MSH 需手写）
 */
#include "FCGmshMeshWriter.h"
#include "FCUnstructuredMeshVTK.h"
#include "FCModelEnum.h"
#include <QFile>
#include <QTextStream>
#include <QFileInfo>

namespace FC {

FCGmshMeshWriter::FCGmshMeshWriter() = default;

FCGmshMeshWriter::~FCGmshMeshWriter() = default;

void FCGmshMeshWriter::setMeshObject(FCUnstructuredMeshVTK* mesh)
{
    _mesh = mesh;
}

void FCGmshMeshWriter::setFileName(const QString& file)
{
    _fileName = file;
}

bool FCGmshMeshWriter::write()
{
    if (_fileName.endsWith(".msh", Qt::CaseInsensitive))
        return writeMSH();
    return writeVTK();
}

bool FCGmshMeshWriter::writeVTK()
{
    if (!_mesh) return false;
    return _mesh->writeToFile(_fileName);
}

bool FCGmshMeshWriter::writeMSH()
{
    if (!_mesh || _fileName.isEmpty()) return false;

    QFile f(_fileName);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

    QTextStream out(&f);

    out << "$MeshFormat\n";
    out << "2.2 0 8\n";
    out << "$EndMeshFormat\n";

    int nNodes = _mesh->getNodeCount();
    out << "$Nodes\n";
    out << nNodes << "\n";
    for (int i = 0; i < nNodes; ++i) {
        FCNode* node = _mesh->getNodeAt(i);
        if (!node) continue;
        double coor[3];
        node->getCoor(coor);
        int id = _mesh->getNodeIDByIndex(i);
        if (id <= 0) id = i + 1;
        out << id << " " << coor[0] << " " << coor[1] << " " << coor[2] << "\n";
    }
    out << "$EndNodes\n";

    int nElems = _mesh->getElementCount();
    out << "$Elements\n";
    out << nElems << "\n";

    static const QHash<FCModelEnum::FITKEleType, int> fcToGmsh = {
        {FCModelEnum::Line2, 1}, {FCModelEnum::Line3, 8},
        {FCModelEnum::Tri3, 2}, {FCModelEnum::Tri6, 9},
        {FCModelEnum::Quad4, 3}, {FCModelEnum::Quad8, 10},
        {FCModelEnum::Tet4, 4}, {FCModelEnum::Tet10, 11},
        {FCModelEnum::Hex8, 5}, {FCModelEnum::Hex20, 12},
        {FCModelEnum::Wedge6, 6},
    };

    for (int i = 0; i < nElems; ++i) {
        FCAbstractElement* ele = _mesh->getElementAt(i);
        if (!ele) continue;
        FCModelEnum::FITKEleType t = ele->getEleType();
        int gmshType = fcToGmsh.value(t, 0);
        if (gmshType == 0) continue;

        int id = ele->getEleID();
        if (id <= 0) id = i + 1;
        int n = ele->getNodeCount();
        out << id << " " << gmshType << " 0";
        for (int j = 0; j < n; ++j)
            out << " " << ele->getNodeID(j);
        out << "\n";
    }
    out << "$EndElements\n";

    f.close();
    return true;
}

} // namespace FC
