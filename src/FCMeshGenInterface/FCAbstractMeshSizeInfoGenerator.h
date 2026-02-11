/**
 * @file FCAbstractMeshSizeInfoGenerator.h
 * @brief 网格尺寸生成器抽象类（移植自 FITKInterfaceMeshGen）
 */
#ifndef FCABSTRACTMESHSIZEINFOGENERATOR_H
#define FCABSTRACTMESHSIZEINFOGENERATOR_H

#include "FCMeshGenInterfaceAPI.h"

namespace FC {

class FCGlobalMeshSizeInfo;
class FCAbstractRegionMeshSize;

/**
 * @brief 网格尺寸生成器抽象类（Gmsh 等实现生成全局/区域尺寸）
 */
class FCMESHGENINTERFACE_API FCAbstractMeshSizeInfoGenerator
{
public:
    explicit FCAbstractMeshSizeInfoGenerator() = default;
    virtual ~FCAbstractMeshSizeInfoGenerator();

    virtual FCGlobalMeshSizeInfo* generateGlobalMeshSizeInfo();
    virtual FCAbstractRegionMeshSize* createRegionMeshSize(int regionType);
};

} // namespace FC

#endif // FCABSTRACTMESHSIZEINFOGENERATOR_H
