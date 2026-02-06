/**
 * @file FCGeoRepairSmallFaces.cpp
 * @brief 修复小面命令实现（接口层）
 */
#include "FCGeoRepairSmallFaces.h"

namespace FC {

FCGeoEnum::FITKGeometryComType FCGeoRepairSmallFaces::getGeometryCommandType()
{
    return FCGeoEnum::FGTRepairSmallFaces;
}

bool FCGeoRepairSmallFaces::update()
{
    return true;
}

} // namespace FC
