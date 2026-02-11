/**
 * @file FCGmshMeshAlgorithmInfoGenerator.cpp
 */
#include "FCGmshMeshAlgorithmInfoGenerator.h"
#include "FCGlobalMeshGenerateAlgorithmInfo.h"

namespace FC {

FCGlobalMeshGenerateAlgorithmInfo* FCGmshMeshAlgorithmInfoGenerator::generateGlobalMeshAlgorithmInfo()
{
    return new FCGlobalMeshGenerateAlgorithmInfo();
}

} // namespace FC
