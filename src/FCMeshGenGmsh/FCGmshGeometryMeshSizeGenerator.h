/**
 * @file FCGmshGeometryMeshSizeGenerator.h
 * @brief Gmsh 几何网格尺寸生成器
 */
#ifndef FCGMSHGEOMETRYMESHSIZEGENERATOR_H
#define FCGMSHGEOMETRYMESHSIZEGENERATOR_H

#include "FCMeshGenGmshAPI.h"
#include "FCAbstractGeometryMeshSizeGenerator.h"

namespace FC {

class FCMESHGENGMSH_API FCGmshGeometryMeshSizeGenerator : public FCAbstractGeometryMeshSizeGenerator
{
public:
    FCGmshGeometryMeshSizeGenerator() = default;
    ~FCGmshGeometryMeshSizeGenerator() override = default;

    FCGeometryMeshSize* createGeometryMeshSize() override;
};

} // namespace FC

#endif // FCGMSHGEOMETRYMESHSIZEGENERATOR_H
