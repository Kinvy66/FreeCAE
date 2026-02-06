/**
 * @file FCGeoVirtualTopoCombine.cpp
 * @brief 虚拓扑合并命令实现（接口层）
 */
#include "FCAbsGeoVirtualTopoCombine.h"

namespace FC {

void FCGeoVirtualTopoCombine::removeFromPart(FCAbsGeoCommand* part)
{
    Q_UNUSED(part);
}

FCGeoEnum::FITKGeometryComType FCGeoVirtualTopoCombineFaces::getGeometryCommandType()
{
    return FCGeoEnum::FGTVirtualTopoCombineFaces;
}

bool FCGeoVirtualTopoCombineFaces::update()
{
    return true;
}

FCGeoEnum::FITKGeometryComType FCGeoVirtualTopoCombineEdges::getGeometryCommandType()
{
    return FCGeoEnum::FGTVirtualTopoCombineEdges;
}

bool FCGeoVirtualTopoCombineEdges::update()
{
    return true;
}

} // namespace FC
