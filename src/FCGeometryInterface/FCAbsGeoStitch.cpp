/**
 * @file FCGeoStitch.cpp
 * @brief 缝合面命令实现（接口层）
 */
#include "FCAbsGeoStitch.h"

namespace FC {

FCGeoEnum::FITKGeometryComType FCGeoStitch::getGeometryCommandType()
{
    return FCGeoEnum::FGTStitch;
}

bool FCGeoStitch::update()
{
    return true;
}

} // namespace FC
