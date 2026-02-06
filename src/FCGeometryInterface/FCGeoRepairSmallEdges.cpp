/**
 * @file FCGeoRepairSmallEdges.cpp
 * @brief 修复小边命令实现（接口层）
 */
#include "FCGeoRepairSmallEdges.h"

namespace FC {

FCGeoEnum::FITKGeometryComType FCGeoRepairSmallEdges::getGeometryCommandType()
{
    return FCGeoEnum::FGTRepairSmallEdges;
}

bool FCGeoRepairSmallEdges::update()
{
    return true;
}

} // namespace FC
