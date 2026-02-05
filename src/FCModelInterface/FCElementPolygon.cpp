/**
 * @file FCElementPolygon.cpp
 * @brief 多边形单元实现（移植自 FITKInterfaceModel）
 */
#include "FCElementPolygon.h"

namespace FC {

FCModelEnum::FITKEleType FCElementPolygon::getEleType()
{
    return FCModelEnum::Polygon;
}

void FCElementPolygon::setNodeCount(int c)
{
    FCAbstractElement::setNodeCount(c);
}

int FCElementPolygon::getNodeCount()
{
    return _elemenTopo.size();
}

int FCElementPolygon::getFaceCount()
{
    return 1;
}

int FCElementPolygon::getElementDim()
{
    return 2;
}

int FCElementPolygon::getEdgeCount()
{
    return _elemenTopo.size();
}

QList<int> FCElementPolygon::getFace(int index, bool lowerOrder)
{
    Q_UNUSED(index);
    Q_UNUSED(lowerOrder);
    return _elemenTopo;
}

QList<int> FCElementPolygon::getEdge(int index)
{
    const int np = _elemenTopo.size();
    if (np == 0) return QList<int>();
    QList<int> edge;
    edge << _elemenTopo[index % np] << _elemenTopo[(index + 1) % np];
    return edge;
}

} // namespace FC
