/**
 * @file FCGeoStiffener.cpp
 * @brief 肋特征命令实现（接口层）
 */
#include "FCGeoStiffener.h"

namespace FC {

FCGeoEnum::FITKGeometryComType FCGeoStiffener::getGeometryCommandType()
{
    return FCGeoEnum::FGTStiffener;
}

bool FCGeoStiffener::update()
{
    return true;
}

} // namespace FC
