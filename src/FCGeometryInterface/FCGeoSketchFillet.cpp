/**
 * @file FCGeoSketchFillet.cpp
 * @brief 草图倒角命令实现（接口层）
 */
#include "FCGeoSketchFillet.h"

namespace FC {

FCGeoEnum::FITKGeometryComType FCGeoSketchFillet::getGeometryCommandType()
{
    return FCGeoEnum::FGTSketchFillet;
}

bool FCGeoSketchFillet::update()
{
    return true;
}

} // namespace FC
