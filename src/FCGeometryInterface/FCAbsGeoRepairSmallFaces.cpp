/**
 * @file FCGeoRepairSmallFaces.cpp
 * @brief 修复小面命令实现（接口层）
 */
#include "FCAbsGeoRepairSmallFaces.h"

namespace FC {

FCGeoEnum::FCGeometryComType FCGeoRepairSmallFaces::getGeometryCommandType()
{
    return FCGeoEnum::FGTRepairSmallFaces;
}

bool FCGeoRepairSmallFaces::update()
{
    return true;
}

} // namespace FC
