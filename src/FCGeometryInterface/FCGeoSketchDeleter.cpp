/**
 * @file FCGeoSketchDeleter.cpp
 * @brief 草图删除命令实现（接口层）
 */
#include "FCGeoSketchDeleter.h"

namespace FC {

FCGeoEnum::FITKGeometryComType FCGeoSketchDeleter::getGeometryCommandType()
{
    return FCGeoEnum::FGTSketchDeleter;
}

bool FCGeoSketchDeleter::update()
{
    return true;
}

} // namespace FC
