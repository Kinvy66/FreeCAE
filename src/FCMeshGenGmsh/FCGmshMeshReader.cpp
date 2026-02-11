/**
 * @file FCGmshMeshReader.cpp
 * @brief Gmsh MSH 文件读取实现
 */
#include "FCGmshMeshReader.h"
#include "FCUnstructuredMeshVTK.h"
#include "FCElementFactory.h"
#include "FCModelEnum.h"
#include <FCData/FCPoint.h>
#include <QFile>
#include <QTextStream>
#include <QHash>

namespace FC {

// Gmsh MSH element type -> FC FITKEleType
static FCModelEnum::FITKEleType gmshEleTypeToFC(int gmshType)
{
    switch (gmshType) {
    case 1:  return FCModelEnum::Line2;      // 2-node line
    case 8:  return FCModelEnum::Line3;     // 3-node line
    case 2:  return FCModelEnum::Tri3;      // 3-node triangle
    case 9:  return FCModelEnum::Tri6;      // 6-node triangle
    case 3:  return FCModelEnum::Quad4;     // 4-node quad
    case 10: return FCModelEnum::Quad4;     // 9-node quad -> Quad4 (取前4角点)
    case 4:  return FCModelEnum::Tet4;      // 4-node tetra
    case 11: return FCModelEnum::Tet10;     // 10-node tetra
    case 5:  return FCModelEnum::Hex8;       // 8-node hex
    case 12: return FCModelEnum::Hex20;     // 20-node hex
    case 6:  return FCModelEnum::Wedge6;    // 6-node prism
    case 13: return FCModelEnum::Wedge6;    // 18-node prism -> Wedge6
    default: return FCModelEnum::EleNone;
    }
}

static int gmshElementNodeCount(int gmshType)
{
    switch (gmshType) {
    case 1: return 2; case 8: return 3;
    case 2: return 3; case 9: return 6;
    case 3: return 4; case 10: return 9;
    case 4: return 4; case 11: return 10;
    case 5: return 8; case 12: return 20;
    case 6: return 6; case 13: return 18;
    default: return 0;
    }
}

// 仅保留体单元 (4,5,6,11,12,13) 及 2D 面单元 (2,3,9,10) 用于表面网格
// 体网格优先
static bool is3DElement(int gmshType)
{
    return gmshType == 4 || gmshType == 5 || gmshType == 6 || gmshType == 11 || gmshType == 12 || gmshType == 13;
}

static bool is2DElement(int gmshType)
{
    return gmshType == 2 || gmshType == 3 || gmshType == 9 || gmshType == 10;
}

FCGmshMeshReader::FCGmshMeshReader() = default;

FCGmshMeshReader::~FCGmshMeshReader() = default;

void FCGmshMeshReader::setFileName(const QString& file)
{
    _fileName = file;
}

void FCGmshMeshReader::setMeshObject(FCUnstructuredMeshVTK* mesh)
{
    _mesh = mesh;
}

int FCGmshMeshReader::parseVersion(QString* errMsg)
{
    QFile f(_fileName);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        if (errMsg) *errMsg = "Cannot open file: " + _fileName;
        return 0;
    }
    QTextStream in(&f);
    QString line = in.readLine().trimmed();
    f.close();
    if (line != "$MeshFormat") return 0;
    line = in.readLine();
    if (line.isNull()) return 0;
    bool ok;
    double ver = line.simplified().split(' ').value(0).toDouble(&ok);
    if (!ok) return 0;
    return (ver >= 4.0) ? 4 : 2;
}

bool FCGmshMeshReader::read()
{
    if (_fileName.isEmpty() || !_mesh) return false;

    QFile f(_fileName);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

    QTextStream in(&f);
    QString line = in.readLine().trimmed();
    if (line != "$MeshFormat") {
        f.close();
        return false;
    }
    line = in.readLine().trimmed();
    QStringList parts = line.split(' ', Qt::SkipEmptyParts);
    if (parts.isEmpty()) { f.close(); return false; }
    bool ok;
    double ver = parts[0].toDouble(&ok);
    if (!ok) { f.close(); return false; }

    // Skip to $EndMeshFormat
    while (!in.atEnd()) {
        if (in.readLine().trimmed() == "$EndMeshFormat") break;
    }

    bool success = false;
    if (ver >= 4.0) {
        f.seek(0);
        success = readMSH4();
    } else {
        f.seek(0);
        success = readMSH2();
    }
    f.close();
    return success;
}

bool FCGmshMeshReader::readMSH2()
{
    QFile f(_fileName);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
    QTextStream in(&f);

    // Skip to $Nodes
    QString line;
    while (!in.atEnd()) {
        line = in.readLine().trimmed();
        if (line == "$Nodes") break;
    }
    if (line != "$Nodes") return false;

    int nNodes = in.readLine().trimmed().toInt();
    QHash<int, int> nodeIdToIndex;
    for (int i = 0; i < nNodes; ++i) {
        line = in.readLine().simplified();
        QStringList parts = line.split(' ', Qt::SkipEmptyParts);
        if (parts.size() < 4) continue;
        int tag = parts[0].toInt();
        double x = parts[1].toDouble();
        double y = parts[2].toDouble();
        double z = parts[3].toDouble();
        FCNode* node = new FCNode(tag, x, y, z);
        _mesh->addNode(node);
        nodeIdToIndex[tag] = _mesh->getNodeCount() - 1;
    }
    // $EndNodes
    while (!in.atEnd() && in.readLine().trimmed() != "$EndNodes") {}

    // $Elements
    while (!in.atEnd()) {
        line = in.readLine().trimmed();
        if (line == "$Elements") break;
    }
    if (line != "$Elements") return true;

    int nElems = in.readLine().trimmed().toInt();
    for (int i = 0; i < nElems; ++i) {
        line = in.readLine().simplified();
        QStringList parts = line.split(' ', Qt::SkipEmptyParts);
        if (parts.size() < 4) continue;
        int elemTag = parts[0].toInt();
        int elemType = parts[1].toInt();
        int nTags = parts[2].toInt();
        int nNodesInElem = parts.size() - 3 - nTags;
        if (nNodesInElem <= 0) continue;

        FCModelEnum::FITKEleType fcType = gmshEleTypeToFC(elemType);
        if (fcType == FCModelEnum::EleNone) continue;
        if (!is3DElement(elemType) && !is2DElement(elemType)) continue;

        FCAbstractElement* ele = FCElementFactory::createElement(fcType);
        if (!ele) continue;

        QList<int> nodeIds;
        int base = 3 + nTags;
        int nUse = (elemType == 10) ? 4 : nNodesInElem;  // 9-node quad -> 4 corners
        for (int j = 0; j < nUse && (base + j) < parts.size(); ++j) {
            int gmshNodeId = parts[base + j].toInt();
            int fcIndex = nodeIdToIndex.value(gmshNodeId, -1);
            if (fcIndex >= 0) {
                nodeIds.append(_mesh->getNodeIDByIndex(fcIndex));
            }
        }
        if (nodeIds.size() == ele->getNodeCount()) {
            ele->setNodeID(nodeIds);
            ele->setEleID(elemTag);
            _mesh->appendElement(ele);
        } else {
            delete ele;
        }
    }

    _mesh->reConstructure();
    return true;
}

bool FCGmshMeshReader::readMSH4()
{
    QFile f(_fileName);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
    QTextStream in(&f);

    QString line;
    while (!in.atEnd()) {
        line = in.readLine().trimmed();
        if (line == "$Nodes") break;
    }
    if (line != "$Nodes") return false;

    // MSH4: numEntityBlocks numNodes minTag maxTag
    line = in.readLine().simplified();
    QStringList parts = line.split(' ', Qt::SkipEmptyParts);
    int numBlocks = parts.value(0).toInt();

    QHash<qint64, int> nodeIdToIndex;
    for (int b = 0; b < numBlocks; ++b) {
        line = in.readLine().simplified();
        parts = line.split(' ', Qt::SkipEmptyParts);
        if (parts.size() < 4) continue;
        int nInBlock = parts[3].toInt();

        // 先读 node tags
        QVector<qint64> tags(nInBlock);
        for (int i = 0; i < nInBlock; ++i) {
            tags[i] = in.readLine().trimmed().toLongLong();
        }
        // 再读坐标 (x y z 或 parametric 时 u v w)
        for (int i = 0; i < nInBlock; ++i) {
            line = in.readLine().simplified();
            parts = line.split(' ', Qt::SkipEmptyParts);
            double x = parts.value(0).toDouble();
            double y = parts.value(1).toDouble();
            double z = parts.size() > 2 ? parts.value(2).toDouble() : 0.0;
            qint64 tag = tags[i];
            FCNode* node = new FCNode(static_cast<int>(tag), x, y, z);
            _mesh->addNode(node);
            nodeIdToIndex[tag] = _mesh->getNodeCount() - 1;
        }
    }
    while (!in.atEnd() && in.readLine().trimmed() != "$EndNodes") {}

    while (!in.atEnd()) {
        line = in.readLine().trimmed();
        if (line == "$Elements") break;
    }
    if (line != "$Elements") return true;

    line = in.readLine().simplified();
    parts = line.split(' ', Qt::SkipEmptyParts);
    numBlocks = parts.value(0).toInt();

    for (int b = 0; b < numBlocks; ++b) {
        line = in.readLine().simplified();
        parts = line.split(' ', Qt::SkipEmptyParts);
        if (parts.size() < 4) continue;
        int elemType = parts[2].toInt();
        int nInBlock = parts[3].toInt();

        int nNodesPerElem = gmshElementNodeCount(elemType);
        if (nNodesPerElem == 0) {
            for (int i = 0; i < nInBlock; ++i) in.readLine();
            continue;
        }

        FCModelEnum::FITKEleType fcType = gmshEleTypeToFC(elemType);
        if (fcType == FCModelEnum::EleNone || (!is3DElement(elemType) && !is2DElement(elemType))) {
            for (int i = 0; i < nInBlock; ++i) in.readLine();
            continue;
        }

        for (int i = 0; i < nInBlock; ++i) {
            line = in.readLine().simplified();
            parts = line.split(' ', Qt::SkipEmptyParts);
            if (parts.size() < nNodesPerElem + 1) continue;
            qint64 elemTag = parts[0].toLongLong();

            int nUse = (elemType == 10) ? 4 : nNodesPerElem;
            QList<int> nodeIds;
            for (int j = 0; j < nUse; ++j) {
                qint64 gmshNodeId = parts[j + 1].toLongLong();
                int fcIndex = nodeIdToIndex.value(gmshNodeId, -1);
                if (fcIndex >= 0) {
                    nodeIds.append(_mesh->getNodeIDByIndex(fcIndex));
                }
            }
            if (nodeIds.size() == nUse) {
                FCAbstractElement* ele = FCElementFactory::createElement(fcType);
                if (ele && ele->getNodeCount() == nUse) {
                    ele->setNodeID(nodeIds);
                    ele->setEleID(static_cast<int>(elemTag));
                    _mesh->appendElement(ele);
                } else if (ele) {
                    delete ele;
                }
            }
        }
    }

    _mesh->reConstructure();
    return true;
}

} // namespace FC
