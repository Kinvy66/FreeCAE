/**
 * @file FCGeoModelOperLine.cpp
 * @brief 线操作命令实现（接口层）
 */
#include "FCAbsGeoModelOperLine.h"

namespace FC {

FCGeoEnum::FCGeometryComType FCGeoModelOperLine::getGeometryCommandType()
{
    switch (m_operType) {
    case GBTMerge: return FCGeoEnum::FGTMergeLine;
    case GBTSplitByMiddlePosition: return FCGeoEnum::FGTSplitByMiddlePositionLine;
    case GBTSplitByAngle: return FCGeoEnum::FGTSplitByAngleLine;
    default: return FCGeoEnum::FGTMergeLine;
    }
}

bool FCGeoModelOperLine::update()
{
    return true;
}

void FCGeoModelOperLine::add(const VirtualShape& shape)
{
    m_shapes.append(shape);
}

void FCGeoModelOperLine::add(const QList<VirtualShape>& shapes)
{
    m_shapes.append(shapes);
}

void FCGeoModelOperLine::set(const QList<VirtualShape>& shapes)
{
    m_shapes = shapes;
}

void FCGeoModelOperLine::remove(int index)
{
    if (index >= 0 && index < m_shapes.size())
        m_shapes.removeAt(index);
}

void FCGeoModelOperLine::clear()
{
    m_shapes.clear();
}

} // namespace FC
