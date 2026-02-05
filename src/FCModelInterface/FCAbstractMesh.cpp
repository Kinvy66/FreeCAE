/**
 * @file FCAbstractMesh.cpp
 * @brief 抽象网格实现（移植自 FITKInterfaceModel）
 */
#include "FCAbstractMesh.h"

namespace FC {

FCAbstractMesh::~FCAbstractMesh()
{}

FCModelEnum::AbsModelType FCAbstractMesh::getAbsModelType()
{
    return FCModelEnum::AMTMesh;
}

FCModelEnum::FITKMeshDim FCAbstractMesh::getMeshDim()
{
    return FCModelEnum::FMDimNone;
}

unsigned int FCAbstractMesh::getMeshDimBit()
{
    return 0;
}

} // namespace FC
