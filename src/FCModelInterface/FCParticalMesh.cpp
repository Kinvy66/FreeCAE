/**
 * @file FCParticalMesh.cpp
 * @brief 粒子网格实现（移植自 FITKInterfaceModel）
 */
#include "FCParticalMesh.h"

namespace FC {

FCModelEnum::AbsModelType FCParticalMesh::getAbsModelType()
{
    return FCModelEnum::AMTPartical;
}

void FCParticalMesh::update()
{}

FCModelEnum::FITKMeshDim FCParticalMesh::getMeshDim()
{
    return FCModelEnum::FMDimD3;
}

} // namespace FC
