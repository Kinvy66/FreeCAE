/**
 * @file FCElementTri.cpp
 * @brief 三角形单元实现（移植自 FITKInterfaceModel）
 */
#include "FCElementTri.h"

namespace FC {

FCElementTri3::FCElementTri3()
{
    setNodeCount(getNodeCount());
}

FCModelEnum::FITKEleType FCElementTri3::getEleType()
{
    return FCModelEnum::Tri3;
}

int FCElementTri3::getNodeCount()
{
    return 3;
}

int FCElementTri3::getFaceCount()
{
    return 5;
}

int FCElementTri3::getEdgeCount()
{
    return 3;
}

QList<int> FCElementTri3::getFace(int index, bool lowerOrder)
{
    Q_UNUSED(lowerOrder);
    QList<int> idList;
    if (index >= getFaceCount() || _elemenTopo.size() < getNodeCount()) return idList;
    switch (index) {
    case 0: idList << _elemenTopo[0] << _elemenTopo[1] << _elemenTopo[2]; break;
    case 1: idList << _elemenTopo[2] << _elemenTopo[1] << _elemenTopo[0]; break;
    case 2: idList << _elemenTopo[0] << _elemenTopo[1]; break;
    case 3: idList << _elemenTopo[1] << _elemenTopo[2]; break;
    case 4: idList << _elemenTopo[2] << _elemenTopo[0]; break;
    }
    return idList;
}

QList<int> FCElementTri3::getEdge(int index)
{
    switch (index) {
    case 0: return QList<int>() << _elemenTopo[0] << _elemenTopo[1];
    case 1: return QList<int>() << _elemenTopo[1] << _elemenTopo[2];
    case 2: return QList<int>() << _elemenTopo[2] << _elemenTopo[0];
    default: return QList<int>();
    }
}

int FCElementTri3::getElementDim()
{
    return 2;
}

FCElementTri6::FCElementTri6()
{
    setNodeCount(getNodeCount());
}

FCModelEnum::FITKEleType FCElementTri6::getEleType()
{
    return FCModelEnum::Tri6;
}

int FCElementTri6::getNodeCount()
{
    return 6;
}

int FCElementTri6::getFaceCount()
{
    return 5;
}

int FCElementTri6::getEdgeCount()
{
    return 3;
}

QList<int> FCElementTri6::getFace(int index, bool lowerOrder)
{
    QList<int> idList;
    if (index >= getFaceCount() || _elemenTopo.size() < getNodeCount()) return idList;
    switch (index) {
    case 0:
        idList << _elemenTopo[0] << _elemenTopo[1] << _elemenTopo[2];
        if (!lowerOrder) idList << _elemenTopo[3] << _elemenTopo[4] << _elemenTopo[5];
        break;
    case 1:
        idList << _elemenTopo[2] << _elemenTopo[1] << _elemenTopo[0];
        if (!lowerOrder) idList << _elemenTopo[4] << _elemenTopo[3] << _elemenTopo[5];
        break;
    case 2:
        idList << _elemenTopo[0] << _elemenTopo[1];
        if (!lowerOrder) idList << _elemenTopo[3];
        break;
    case 3:
        idList << _elemenTopo[1] << _elemenTopo[2];
        if (!lowerOrder) idList << _elemenTopo[4];
        break;
    case 4:
        idList << _elemenTopo[2] << _elemenTopo[0];
        if (!lowerOrder) idList << _elemenTopo[5];
        break;
    }
    return idList;
}

QList<int> FCElementTri6::getEdge(int index)
{
    switch (index) {
    case 0: return QList<int>() << _elemenTopo[0] << _elemenTopo[3] << _elemenTopo[1];
    case 1: return QList<int>() << _elemenTopo[1] << _elemenTopo[4] << _elemenTopo[2];
    case 2: return QList<int>() << _elemenTopo[2] << _elemenTopo[5] << _elemenTopo[0];
    default: return QList<int>();
    }
}

int FCElementTri6::getElementDim()
{
    return 2;
}

} // namespace FC
