/**
 * @file FCGeoModelImport.cpp
 * @brief 几何导入命令实现（接口层）
 */
#include "FCAbsGeoModelImport.h"

namespace FC {

FCGeoEnum::FCGeometryComType FCGeoModelImport::getGeometryCommandType()
{
    return FCGeoEnum::FGTImport;
}

bool FCGeoModelImport::update()
{
    return true;
}

} // namespace FC
