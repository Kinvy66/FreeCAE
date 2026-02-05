/**
 * @file FCElementLine.cpp
 * @brief 线单元实现（移植自 FITKInterfaceModel）
 */
#include "FCElementLine.h"

namespace FC {

FCElementLine2::FCElementLine2()
{
    setNodeCount(getNodeCount());
}

FCModelEnum::FITKEleType FCElementLine2::getEleType()
{
    return FCModelEnum::Line2;
}

int FCElementLine2::getNodeCount()
{
    return 2;
}

int FCElementLine2::getFaceCount()
{
    return 0;
}

int FCElementLine2::getEdgeCount()
{
    return 1;
}

QList<int> FCElementLine2::getFace(int index, bool lowerOrder)
{
    Q_UNUSED(index);
    Q_UNUSED(lowerOrder);
    return QList<int>();
}

QList<int> FCElementLine2::getEdge(int index)
{
    Q_UNUSED(index);
    return _elemenTopo;
}

int FCElementLine2::getElementDim()
{
    return 1;
}

FCElementLine3::FCElementLine3()
{
    setNodeCount(getNodeCount());
}

FCModelEnum::FITKEleType FCElementLine3::getEleType()
{
    return FCModelEnum::Line3;
}

int FCElementLine3::getNodeCount()
{
    return 3;
}

int FCElementLine3::getFaceCount()
{
    return 0;
}

int FCElementLine3::getEdgeCount()
{
    return 1;
}

QList<int> FCElementLine3::getFace(int index, bool lowerOrder)
{
    Q_UNUSED(index);
    Q_UNUSED(lowerOrder);
    return QList<int>();
}

QList<int> FCElementLine3::getEdge(int index)
{
    Q_UNUSED(index);
    return _elemenTopo;
}

int FCElementLine3::getElementDim()
{
    return 1;
}

} // namespace FC
