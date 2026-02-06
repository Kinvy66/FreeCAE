/**
 * @file FCGeoCoverEdges.cpp
 * @brief 覆盖面命令实现（接口层）
 */
#include "FCGeoCoverEdges.h"

namespace FC {

FCGeoEnum::FITKGeometryComType FCGeoCoverEdges::getGeometryCommandType()
{
    return FCGeoEnum::FGTCoverEdges;
}

bool FCGeoCoverEdges::update()
{
    return true;
}

} // namespace FC
