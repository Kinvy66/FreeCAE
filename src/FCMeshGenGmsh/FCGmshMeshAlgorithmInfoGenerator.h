/**
 * @file FCGmshMeshAlgorithmInfoGenerator.h
 * @brief Gmsh 网格算法信息生成器
 */
#ifndef FCGMSHMESHALGORITHMINFOGENERATOR_H
#define FCGMSHMESHALGORITHMINFOGENERATOR_H

#include "FCMeshGenGmshAPI.h"
#include "FCAbstractMeshAlgorithmInfoGenerator.h"

namespace FC {

class FCMESHGENGMSH_API FCGmshMeshAlgorithmInfoGenerator : public FCAbstractMeshAlgorithmInfoGenerator
{
public:
    FCGmshMeshAlgorithmInfoGenerator() = default;
    ~FCGmshMeshAlgorithmInfoGenerator() override = default;

    FCGlobalMeshGenerateAlgorithmInfo* generateGlobalMeshAlgorithmInfo() override;
};

} // namespace FC

#endif // FCGMSHMESHALGORITHMINFOGENERATOR_H
