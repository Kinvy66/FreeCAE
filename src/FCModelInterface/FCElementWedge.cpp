/**
 * @file FCElementWedge.cpp
 * @brief 三棱柱单元实现（移植自 FITKInterfaceModel）
 */
#include "FCElementWedge.h"

namespace FC {

FCElementWedge6::FCElementWedge6()
{
    setNodeCount(getNodeCount());
}

FCModelEnum::FITKEleType FCElementWedge6::getEleType()
{
    return FCModelEnum::Wedge6;
}

int FCElementWedge6::getNodeCount()
{
    return 6;
}

int FCElementWedge6::getFaceCount()
{
    return 5;
}

int FCElementWedge6::getEdgeCount()
{
    return 9;
}

QList<int> FCElementWedge6::getFace(int index, bool lowerOrder)
{
    Q_UNUSED(lowerOrder);
    QList<int> idList;
    if (index >= getFaceCount() || _elemenTopo.size() < getNodeCount()) return idList;
    switch (index) {
    case 0: idList << _elemenTopo[2] << _elemenTopo[1] << _elemenTopo[0]; break;
    case 1: idList << _elemenTopo[3] << _elemenTopo[4] << _elemenTopo[5]; break;
    case 2: idList << _elemenTopo[4] << _elemenTopo[3] << _elemenTopo[0] << _elemenTopo[1]; break;
    case 3: idList << _elemenTopo[5] << _elemenTopo[4] << _elemenTopo[1] << _elemenTopo[2]; break;
    case 4: idList << _elemenTopo[0] << _elemenTopo[3] << _elemenTopo[5] << _elemenTopo[2]; break;
    }
    return idList;
}

QList<int> FCElementWedge6::getEdge(int index)
{
    switch (index) {
    case 0: return QList<int>() << _elemenTopo[0] << _elemenTopo[1];
    case 1: return QList<int>() << _elemenTopo[1] << _elemenTopo[2];
    case 2: return QList<int>() << _elemenTopo[2] << _elemenTopo[0];
    case 3: return QList<int>() << _elemenTopo[3] << _elemenTopo[4];
    case 4: return QList<int>() << _elemenTopo[4] << _elemenTopo[5];
    case 5: return QList<int>() << _elemenTopo[5] << _elemenTopo[3];
    case 6: return QList<int>() << _elemenTopo[0] << _elemenTopo[3];
    case 7: return QList<int>() << _elemenTopo[1] << _elemenTopo[4];
    case 8: return QList<int>() << _elemenTopo[2] << _elemenTopo[5];
    default: return QList<int>();
    }
}

int FCElementWedge6::getElementDim()
{
    return 3;
}

} // namespace FC
