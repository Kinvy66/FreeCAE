/**
 * @file FCAbstractMeshSizeInfoGenerator.cpp
 */
#include "FCAbstractMeshSizeInfoGenerator.h"
#include "FCGlobalMeshSizeInfo.h"
#include "FCAbstractRegionMeshSize.h"
#include "FCRegionMeshSizeBox.h"

namespace FC {

FCAbstractMeshSizeInfoGenerator::~FCAbstractMeshSizeInfoGenerator() {}

FCGlobalMeshSizeInfo* FCAbstractMeshSizeInfoGenerator::generateGlobalMeshSizeInfo()
{
    return new FCGlobalMeshSizeInfo();
}

FCAbstractRegionMeshSize* FCAbstractMeshSizeInfoGenerator::createRegionMeshSize(int regionType)
{
    if (regionType == FCAbstractRegionMeshSize::RegionBox)
        return new FCRegionMeshSizeBox();
    return nullptr;  // 其他类型由 Gmsh 等子类实现
}

} // namespace FC
