/**
 * @file FCGeoOperNBodyUnite.cpp
 * @brief 多实体布尔求和命令实现（接口层）
 */
#include "FCGeoOperNBodyUnite.h"

namespace FC {

FCGeoEnum::FITKGeometryComType FCGeoOperNBodyUnite::getGeometryCommandType()
{
    return FCGeoEnum::FGTNBodyUnite;
}

bool FCGeoOperNBodyUnite::update()
{
    return true;
}

void FCGeoOperNBodyUnite::addShape(const VirtualShape& shape)
{
    if (shape.Type != FCGeoEnum::VSSolid)
        return;
    for (const VirtualShape& iShape : m_shapes) {
        if (iShape.CmdId == shape.CmdId &&
            (iShape.VirtualTopoId == shape.VirtualTopoId || iShape.VirtualTopoIndex == shape.VirtualTopoIndex))
            return;
    }
    m_shapes.append(shape);
}

void FCGeoOperNBodyUnite::clearShapes()
{
    m_shapes.clear();
}

} // namespace FC
