/**
 * @file FCGeoModelImportFlattenAssembly.cpp
 * @brief 拆分扁平装配模式导入实现（接口层）
 */
#include "FCAbsGeoModelImportFlattenAssembly.h"

namespace FC {

FCGeoEnum::FCGeometryComType FCGeoModelImportFlattenAssembly::getGeometryCommandType()
{
    return FCGeoEnum::FGTImportFlattenAssembly;
}

bool FCGeoModelImportFlattenAssembly::update()
{
    return true;
}

} // namespace FC
