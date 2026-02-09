/**
 * @file FCGeoModelOperImprintSolid.cpp
 * @brief 压印体操作命令实现（接口层）
 */
#include "FCAbsGeoModelOperImprintSolid.h"

namespace FC {

FCGeoEnum::FCGeometryComType FCGeoModelOperImprintSolid::getGeometryCommandType()
{
    return FCGeoEnum::FGTImprintSolid;
}

bool FCGeoModelOperImprintSolid::update()
{
    return true;
}

void FCGeoModelOperImprintSolid::add(const VirtualShape& shape)
{
    m_shapes.append(shape);
}

void FCGeoModelOperImprintSolid::set(const VirtualShape& shape)
{
    m_shapes = QList<VirtualShape>() << shape;
}

void FCGeoModelOperImprintSolid::add(const QList<VirtualShape>& shapes)
{
    m_shapes.append(shapes);
}

void FCGeoModelOperImprintSolid::set(const QList<VirtualShape>& shapes)
{
    m_shapes = shapes;
}

void FCGeoModelOperImprintSolid::remove(int index)
{
    if (index >= 0 && index < m_shapes.size())
        m_shapes.removeAt(index);
}

void FCGeoModelOperImprintSolid::clear()
{
    m_shapes.clear();
}

} // namespace FC
