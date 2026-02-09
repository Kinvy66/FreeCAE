/**
 * @file FCGeoModelOperFace.cpp
 * @brief 面操作命令实现（接口层）
 */
#include "FCAbsGeoModelOperFace.h"

namespace FC {

FCGeoEnum::FCGeometryComType FCGeoModelOperFace::getGeometryCommandType()
{
    switch (m_operType) {
    case GBTFillHoles: return FCGeoEnum::FGTFillHolesFace;
    case GBTFillGaps: return FCGeoEnum::FGTFillGapsFace;
    case GBTDeleteFloatingEdge: return FCGeoEnum::FGTDeleteFloatingEdge;
    case GBTImprintMapPointToFace: return FCGeoEnum::FGTImprintMapPointToFace;
    case GBTImprintMapLineToFace: return FCGeoEnum::FGTImprintMapLineToFace;
    default: return FCGeoEnum::FGTFillHolesFace;
    }
}

bool FCGeoModelOperFace::update()
{
    return true;
}

void FCGeoModelOperFace::add(const VirtualShape& shape)
{
    m_shapes.append(shape);
}

void FCGeoModelOperFace::add(const QList<VirtualShape>& shapes)
{
    m_shapes.append(shapes);
}

void FCGeoModelOperFace::set(const QList<VirtualShape>& shapes)
{
    m_shapes = shapes;
}

void FCGeoModelOperFace::remove(int index)
{
    if (index >= 0 && index < m_shapes.size())
        m_shapes.removeAt(index);
}

void FCGeoModelOperFace::clear()
{
    m_shapes.clear();
}

FCGeoEnum::FCGeometryComType FCGeoModelOperFaceRemoveFace::getGeometryCommandType()
{
    return FCGeoEnum::FGTRemoveFace;
}
bool FCGeoModelOperFaceRemoveFace::update() { return true; }

FCGeoEnum::FCGeometryComType FCGeoModelOperFaceExtendFace::getGeometryCommandType()
{
    return FCGeoEnum::FGTExtendFace;
}
bool FCGeoModelOperFaceExtendFace::update() { return true; }

} // namespace FC
