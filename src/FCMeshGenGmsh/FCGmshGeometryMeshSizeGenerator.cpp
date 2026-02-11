/**
 * @file FCGmshGeometryMeshSizeGenerator.cpp
 */
#include "FCGmshGeometryMeshSizeGenerator.h"
#include "FCGeometryMeshSize.h"

namespace FC {

FCGeometryMeshSize* FCGmshGeometryMeshSizeGenerator::createGeometryMeshSize()
{
    return new FCGeometryMeshSize();
}

} // namespace FC
