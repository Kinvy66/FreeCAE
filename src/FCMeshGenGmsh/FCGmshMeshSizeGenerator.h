/**
 * @file FCGmshMeshSizeGenerator.h
 * @brief Gmsh 网格尺寸生成器
 */
#ifndef FCGMSHMESHSIZEGENERATOR_H
#define FCGMSHMESHSIZEGENERATOR_H

#include "FCMeshGenGmshAPI.h"
#include "FCAbstractMeshSizeInfoGenerator.h"

namespace FC {

class FCMESHGENGMSH_API FCGmshMeshSizeGenerator : public FCAbstractMeshSizeInfoGenerator
{
public:
    FCGmshMeshSizeGenerator() = default;
    ~FCGmshMeshSizeGenerator() override = default;

    FCGlobalMeshSizeInfo* generateGlobalMeshSizeInfo() override;
    FCAbstractRegionMeshSize* createRegionMeshSize(int regionType) override;
};

} // namespace FC

#endif // FCGMSHMESHSIZEGENERATOR_H
