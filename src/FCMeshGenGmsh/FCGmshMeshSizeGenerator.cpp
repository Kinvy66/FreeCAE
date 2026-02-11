/**
 * @file FCGmshMeshSizeGenerator.cpp
 */
#include "FCGmshMeshSizeGenerator.h"
#include "FCGlobalMeshSizeInfo.h"
#include "FCAbstractRegionMeshSize.h"
#include "FCRegionMeshSizeBox.h"

namespace FC {

FCGlobalMeshSizeInfo* FCGmshMeshSizeGenerator::generateGlobalMeshSizeInfo()
{
    return new FCGlobalMeshSizeInfo();
}

FCAbstractRegionMeshSize* FCGmshMeshSizeGenerator::createRegionMeshSize(int regionType)
{
    if (regionType == FCAbstractRegionMeshSize::RegionBox)
        return new FCRegionMeshSizeBox();
    return nullptr;
}

} // namespace FC
