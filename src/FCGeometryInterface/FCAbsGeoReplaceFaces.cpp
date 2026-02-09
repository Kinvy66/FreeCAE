/**
 * @file FCGeoReplaceFaces.cpp
 * @brief 替换面命令实现（接口层）
 */
#include "FCAbsGeoReplaceFaces.h"

namespace FC {

FCGeoEnum::FCGeometryComType FCGeoReplaceFaces::getGeometryCommandType()
{
    return FCGeoEnum::FGTReplaceFaces;
}

bool FCGeoReplaceFaces::update()
{
    return true;
}

} // namespace FC
