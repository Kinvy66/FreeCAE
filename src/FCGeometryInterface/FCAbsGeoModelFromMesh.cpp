/**
 * @file FCGeoModelFromMesh.cpp
 * @brief 从网格生成几何命令实现（接口层）
 */
#include "FCAbsGeoModelFromMesh.h"

namespace FC {

FCGeoEnum::FCGeometryComType FCGeoModelFromMesh::getGeometryCommandType()
{
    return FCGeoEnum::FGTModelFromMesh;
}

bool FCGeoModelFromMesh::update()
{
    return true;
}

} // namespace FC
