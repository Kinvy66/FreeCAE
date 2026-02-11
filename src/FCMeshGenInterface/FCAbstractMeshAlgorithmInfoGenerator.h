/**
 * @file FCAbstractMeshAlgorithmInfoGenerator.h
 * @brief 网格算法生成器抽象类（移植自 FITKInterfaceMeshGen）
 */
#ifndef FCABSTRACTMESHALGORITHMINFOGENERATOR_H
#define FCABSTRACTMESHALGORITHMINFOGENERATOR_H

#include "FCMeshGenInterfaceAPI.h"

namespace FC {

class FCGlobalMeshGenerateAlgorithmInfo;

/**
 * @brief 网格算法生成器抽象类（Gmsh 等实现）
 */
class FCMESHGENINTERFACE_API FCAbstractMeshAlgorithmInfoGenerator
{
public:
    explicit FCAbstractMeshAlgorithmInfoGenerator() = default;
    virtual ~FCAbstractMeshAlgorithmInfoGenerator();

    virtual FCGlobalMeshGenerateAlgorithmInfo* generateGlobalMeshAlgorithmInfo();
};

} // namespace FC

#endif // FCABSTRACTMESHALGORITHMINFOGENERATOR_H
