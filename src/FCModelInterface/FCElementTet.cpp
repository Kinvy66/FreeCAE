/**
 * @file FCElementTet.cpp
 * @brief 四面体单元实现（移植自 FITKInterfaceModel）
 */
#include "FCElementTet.h"

namespace FC {

FCElementTet4::FCElementTet4()
{
    setNodeCount(getNodeCount());
}

FCModelEnum::FITKEleType FCElementTet4::getEleType()
{
    return FCModelEnum::Tet4;
}

int FCElementTet4::getNodeCount()
{
    return 4;
}

int FCElementTet4::getFaceCount()
{
    return 4;
}

int FCElementTet4::getEdgeCount()
{
    return 6;
}

QList<int> FCElementTet4::getFace(int index, bool lowerOrder)
{
    Q_UNUSED(lowerOrder);
    QList<int> idList;
    if (index >= getFaceCount() || _elemenTopo.size() < getNodeCount()) return idList;
    switch (index) {
    case 0: idList << _elemenTopo[2] << _elemenTopo[1] << _elemenTopo[0]; break;
    case 1: idList << _elemenTopo[0] << _elemenTopo[1] << _elemenTopo[3]; break;
    case 2: idList << _elemenTopo[1] << _elemenTopo[2] << _elemenTopo[3]; break;
    case 3: idList << _elemenTopo[0] << _elemenTopo[2] << _elemenTopo[3]; break;
    }
    return idList;
}

QList<int> FCElementTet4::getEdge(int index)
{
    switch (index) {
    case 0: return QList<int>() << _elemenTopo[0] << _elemenTopo[1];
    case 1: return QList<int>() << _elemenTopo[1] << _elemenTopo[2];
    case 2: return QList<int>() << _elemenTopo[2] << _elemenTopo[0];
    case 3: return QList<int>() << _elemenTopo[0] << _elemenTopo[3];
    case 4: return QList<int>() << _elemenTopo[1] << _elemenTopo[3];
    case 5: return QList<int>() << _elemenTopo[2] << _elemenTopo[3];
    default: return QList<int>();
    }
}

int FCElementTet4::getElementDim()
{
    return 3;
}

FCElementTet10::FCElementTet10()
{
    setNodeCount(getNodeCount());
}

FCModelEnum::FITKEleType FCElementTet10::getEleType()
{
    return FCModelEnum::Tet10;
}

int FCElementTet10::getNodeCount()
{
    return 10;
}

int FCElementTet10::getFaceCount()
{
    return 4;
}

int FCElementTet10::getEdgeCount()
{
    return 6;
}

QList<int> FCElementTet10::getFace(int index, bool lowerOrder)
{
    QList<int> idList;
    if (index >= getFaceCount() || _elemenTopo.size() < getNodeCount()) return idList;
    switch (index) {
    case 0:
        idList << _elemenTopo[2] << _elemenTopo[1] << _elemenTopo[0];
        if (!lowerOrder) idList << _elemenTopo[5] << _elemenTopo[4] << _elemenTopo[6];
        break;
    case 1:
        idList << _elemenTopo[0] << _elemenTopo[1] << _elemenTopo[3];
        if (!lowerOrder) idList << _elemenTopo[4] << _elemenTopo[8] << _elemenTopo[7];
        break;
    case 2:
        idList << _elemenTopo[1] << _elemenTopo[2] << _elemenTopo[3];
        if (!lowerOrder) idList << _elemenTopo[5] << _elemenTopo[9] << _elemenTopo[8];
        break;
    case 3:
        idList << _elemenTopo[0] << _elemenTopo[2] << _elemenTopo[3];
        if (!lowerOrder) idList << _elemenTopo[6] << _elemenTopo[9] << _elemenTopo[7];
        break;
    }
    return idList;
}

QList<int> FCElementTet10::getEdge(int index)
{
    switch (index) {
    case 0: return QList<int>() << _elemenTopo[0] << _elemenTopo[4] << _elemenTopo[1];
    case 1: return QList<int>() << _elemenTopo[1] << _elemenTopo[5] << _elemenTopo[2];
    case 2: return QList<int>() << _elemenTopo[2] << _elemenTopo[6] << _elemenTopo[0];
    case 3: return QList<int>() << _elemenTopo[0] << _elemenTopo[7] << _elemenTopo[3];
    case 4: return QList<int>() << _elemenTopo[1] << _elemenTopo[8] << _elemenTopo[3];
    case 5: return QList<int>() << _elemenTopo[2] << _elemenTopo[9] << _elemenTopo[3];
    default: return QList<int>();
    }
}

int FCElementTet10::getElementDim()
{
    return 3;
}

} // namespace FC
