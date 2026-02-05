/**
 * @file FCElementQuad.cpp
 * @brief 四边形单元实现（移植自 FITKInterfaceModel）
 */
#include "FCElementQuad.h"

namespace FC {

FCElementQuad4::FCElementQuad4()
{
    setNodeCount(getNodeCount());
}

FCModelEnum::FITKEleType FCElementQuad4::getEleType()
{
    return FCModelEnum::Quad4;
}

int FCElementQuad4::getNodeCount()
{
    return 4;
}

int FCElementQuad4::getFaceCount()
{
    return 6;
}

int FCElementQuad4::getEdgeCount()
{
    return 4;
}

QList<int> FCElementQuad4::getFace(int index, bool lowerOrder)
{
    Q_UNUSED(lowerOrder);
    QList<int> idList;
    if (index >= getFaceCount() || _elemenTopo.size() < getNodeCount()) return idList;
    switch (index) {
    case 0: idList << _elemenTopo[0] << _elemenTopo[1] << _elemenTopo[2] << _elemenTopo[3]; break;
    case 1: idList << _elemenTopo[3] << _elemenTopo[2] << _elemenTopo[1] << _elemenTopo[0]; break;
    case 2: idList << _elemenTopo[0] << _elemenTopo[1]; break;
    case 3: idList << _elemenTopo[1] << _elemenTopo[2]; break;
    case 4: idList << _elemenTopo[2] << _elemenTopo[3]; break;
    case 5: idList << _elemenTopo[3] << _elemenTopo[0]; break;
    }
    return idList;
}

QList<int> FCElementQuad4::getEdge(int index)
{
    switch (index) {
    case 0: return QList<int>() << _elemenTopo[0] << _elemenTopo[1];
    case 1: return QList<int>() << _elemenTopo[1] << _elemenTopo[2];
    case 2: return QList<int>() << _elemenTopo[2] << _elemenTopo[3];
    case 3: return QList<int>() << _elemenTopo[3] << _elemenTopo[0];
    default: return QList<int>();
    }
}

int FCElementQuad4::getElementDim()
{
    return 2;
}

FCElementQuad8::FCElementQuad8()
{
    setNodeCount(getNodeCount());
}

FCModelEnum::FITKEleType FCElementQuad8::getEleType()
{
    return FCModelEnum::Quad8;
}

int FCElementQuad8::getNodeCount()
{
    return 8;
}

int FCElementQuad8::getFaceCount()
{
    return 6;
}

int FCElementQuad8::getEdgeCount()
{
    return 4;
}

QList<int> FCElementQuad8::getFace(int index, bool lowerOrder)
{
    QList<int> idList;
    if (index >= getFaceCount() || _elemenTopo.size() < getNodeCount()) return idList;
    switch (index) {
    case 0:
        idList << _elemenTopo[0] << _elemenTopo[1] << _elemenTopo[2] << _elemenTopo[3];
        if (!lowerOrder) idList << _elemenTopo[4] << _elemenTopo[5] << _elemenTopo[6] << _elemenTopo[7];
        break;
    case 1:
        idList << _elemenTopo[3] << _elemenTopo[2] << _elemenTopo[1] << _elemenTopo[0];
        if (!lowerOrder) idList << _elemenTopo[6] << _elemenTopo[5] << _elemenTopo[4] << _elemenTopo[7];
        break;
    case 2:
        idList << _elemenTopo[0] << _elemenTopo[1];
        if (!lowerOrder) idList << _elemenTopo[4];
        break;
    case 3:
        idList << _elemenTopo[1] << _elemenTopo[2];
        if (!lowerOrder) idList << _elemenTopo[5];
        break;
    case 4:
        idList << _elemenTopo[2] << _elemenTopo[3];
        if (!lowerOrder) idList << _elemenTopo[6];
        break;
    case 5:
        idList << _elemenTopo[3] << _elemenTopo[0];
        if (!lowerOrder) idList << _elemenTopo[7];
        break;
    }
    return idList;
}

QList<int> FCElementQuad8::getEdge(int index)
{
    switch (index) {
    case 0: return QList<int>() << _elemenTopo[0] << _elemenTopo[4] << _elemenTopo[1];
    case 1: return QList<int>() << _elemenTopo[1] << _elemenTopo[5] << _elemenTopo[2];
    case 2: return QList<int>() << _elemenTopo[2] << _elemenTopo[6] << _elemenTopo[3];
    case 3: return QList<int>() << _elemenTopo[3] << _elemenTopo[7] << _elemenTopo[0];
    default: return QList<int>();
    }
}

int FCElementQuad8::getElementDim()
{
    return 2;
}

} // namespace FC
