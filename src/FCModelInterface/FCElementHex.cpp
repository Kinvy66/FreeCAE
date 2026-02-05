/**
 * @file FCElementHex.cpp
 * @brief 六面体单元实现（移植自 FITKInterfaceModel）
 */
#include "FCElementHex.h"

namespace FC {

FCElementHex8::FCElementHex8()
{
    setNodeCount(getNodeCount());
}

FCModelEnum::FITKEleType FCElementHex8::getEleType()
{
    return FCModelEnum::Hex8;
}

int FCElementHex8::getNodeCount()
{
    return 8;
}

int FCElementHex8::getFaceCount()
{
    return 6;
}

int FCElementHex8::getEdgeCount()
{
    return 12;
}

QList<int> FCElementHex8::getFace(int index, bool lowerOrder)
{
    Q_UNUSED(lowerOrder);
    QList<int> idList;
    if (index >= getFaceCount() || _elemenTopo.size() < getNodeCount()) return idList;
    switch (index) {
    case 0: idList << _elemenTopo[3] << _elemenTopo[2] << _elemenTopo[1] << _elemenTopo[0]; break;
    case 1: idList << _elemenTopo[4] << _elemenTopo[5] << _elemenTopo[6] << _elemenTopo[7]; break;
    case 2: idList << _elemenTopo[0] << _elemenTopo[1] << _elemenTopo[5] << _elemenTopo[4]; break;
    case 3: idList << _elemenTopo[1] << _elemenTopo[2] << _elemenTopo[6] << _elemenTopo[5]; break;
    case 4: idList << _elemenTopo[2] << _elemenTopo[3] << _elemenTopo[7] << _elemenTopo[6]; break;
    case 5: idList << _elemenTopo[0] << _elemenTopo[4] << _elemenTopo[7] << _elemenTopo[3]; break;
    }
    return idList;
}

QList<int> FCElementHex8::getEdge(int index)
{
    switch (index) {
    case 0: return QList<int>() << _elemenTopo[0] << _elemenTopo[1];
    case 1: return QList<int>() << _elemenTopo[1] << _elemenTopo[2];
    case 2: return QList<int>() << _elemenTopo[2] << _elemenTopo[3];
    case 3: return QList<int>() << _elemenTopo[3] << _elemenTopo[0];
    case 4: return QList<int>() << _elemenTopo[4] << _elemenTopo[5];
    case 5: return QList<int>() << _elemenTopo[5] << _elemenTopo[6];
    case 6: return QList<int>() << _elemenTopo[6] << _elemenTopo[7];
    case 7: return QList<int>() << _elemenTopo[7] << _elemenTopo[4];
    case 8: return QList<int>() << _elemenTopo[0] << _elemenTopo[4];
    case 9: return QList<int>() << _elemenTopo[1] << _elemenTopo[5];
    case 10: return QList<int>() << _elemenTopo[2] << _elemenTopo[6];
    case 11: return QList<int>() << _elemenTopo[3] << _elemenTopo[7];
    default: return QList<int>();
    }
}

int FCElementHex8::getElementDim()
{
    return 3;
}

FCElementHex20::FCElementHex20()
{
    setNodeCount(getNodeCount());
}

FCModelEnum::FITKEleType FCElementHex20::getEleType()
{
    return FCModelEnum::Hex20;
}

int FCElementHex20::getNodeCount()
{
    return 20;
}

int FCElementHex20::getFaceCount()
{
    return 6;
}

int FCElementHex20::getEdgeCount()
{
    return 12;
}

QList<int> FCElementHex20::getFace(int index, bool lowerOrder)
{
    Q_UNUSED(index);
    Q_UNUSED(lowerOrder);
    return QList<int>();
}

QList<int> FCElementHex20::getEdge(int index)
{
    Q_UNUSED(index);
    return QList<int>();
}

int FCElementHex20::getElementDim()
{
    return 3;
}

} // namespace FC
