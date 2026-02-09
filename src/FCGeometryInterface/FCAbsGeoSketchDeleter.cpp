/**
 * @file FCGeoSketchDeleter.cpp
 * @brief 草图删除命令实现（接口层）
 */
#include "FCAbsGeoSketchDeleter.h"

namespace FC {

FCGeoEnum::FCGeometryComType FCGeoSketchDeleter::getGeometryCommandType()
{
    return FCGeoEnum::FGTSketchDeleter;
}

bool FCGeoSketchDeleter::update()
{
    return true;
}

} // namespace FC
