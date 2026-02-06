/**
 * @file FCGeoModelImportFlattenAssembly.cpp
 * @brief 拆分平铺装配模式导入实现（接口层�?
 */
#include "FCAbsGeoModelImportFlattenAssembly.h"

namespace FC {

FCGeoEnum::FITKGeometryComType FCGeoModelImportFlattenAssembly::getGeometryCommandType()
{
    return FCGeoEnum::FGTImportFlattenAssembly;
}

bool FCGeoModelImportFlattenAssembly::update()
{
    return true;
}

} // namespace FC
