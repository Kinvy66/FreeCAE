/**
 * @file FCAbstractGeometryMeshSizeGenerator.cpp
 */
#include "FCAbstractGeometryMeshSizeGenerator.h"
#include "FCGeometryMeshSize.h"

namespace FC {

FCAbstractGeometryMeshSizeGenerator::~FCAbstractGeometryMeshSizeGenerator() {}

FCGeometryMeshSize* FCAbstractGeometryMeshSizeGenerator::createGeometryMeshSize()
{
    return new FCGeometryMeshSize();
}

} // namespace FC
