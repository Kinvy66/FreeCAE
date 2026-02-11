/**
 * @file FCAbstractGeometryMeshSizeGenerator.h
 * @brief 几何网格尺寸生成器抽象类（移植自 FITKInterfaceMeshGen）
 */
#ifndef FCABSTRACTGEOMETRYMESHSIZEGENERATOR_H
#define FCABSTRACTGEOMETRYMESHSIZEGENERATOR_H

#include "FCMeshGenInterfaceAPI.h"

namespace FC {

class FCGeometryMeshSize;

/**
 * @brief 几何网格尺寸生成器抽象类（Gmsh 等实现）
 */
class FCMESHGENINTERFACE_API FCAbstractGeometryMeshSizeGenerator
{
public:
    explicit FCAbstractGeometryMeshSizeGenerator() = default;
    virtual ~FCAbstractGeometryMeshSizeGenerator();

    virtual FCGeometryMeshSize* createGeometryMeshSize();
};

} // namespace FC

#endif // FCABSTRACTGEOMETRYMESHSIZEGENERATOR_H
