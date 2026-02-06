/**
 * @file FCGeoReplaceFaces.cpp
 * @brief 替换面命令实现（接口层）
 */
#include "FCGeoReplaceFaces.h"

namespace FC {

FCGeoEnum::FITKGeometryComType FCGeoReplaceFaces::getGeometryCommandType()
{
    return FCGeoEnum::FGTReplaceFaces;
}

bool FCGeoReplaceFaces::update()
{
    return true;
}

} // namespace FC
