/**
 * @file FCGeoModelFromMesh.cpp
 * @brief 从网格生成几何命令实现（接口层）
 */
#include "FCGeoModelFromMesh.h"

namespace FC {

FCGeoEnum::FITKGeometryComType FCGeoModelFromMesh::getGeometryCommandType()
{
    return FCGeoEnum::FGTModelFromMesh;
}

bool FCGeoModelFromMesh::update()
{
    return true;
}

} // namespace FC
