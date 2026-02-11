/**
 * @file FCAbstractMeshAlgorithmInfoGenerator.cpp
 */
#include "FCAbstractMeshAlgorithmInfoGenerator.h"
#include "FCGlobalMeshGenerateAlgorithmInfo.h"

namespace FC {

FCAbstractMeshAlgorithmInfoGenerator::~FCAbstractMeshAlgorithmInfoGenerator() {}

FCGlobalMeshGenerateAlgorithmInfo* FCAbstractMeshAlgorithmInfoGenerator::generateGlobalMeshAlgorithmInfo()
{
    return new FCGlobalMeshGenerateAlgorithmInfo();
}

} // namespace FC
