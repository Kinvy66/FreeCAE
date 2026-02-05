/**
 * @file FCStructuredMesh.cpp
 * @brief 结构网格实现（移植自 FITKInterfaceModel）
 */
#include "FCStructuredMesh.h"

namespace FC {

void FCStructuredMesh::setIJKDim(int i, int j, int k)
{
    _ijkDim[0] = i;
    _ijkDim[1] = j;
    _ijkDim[2] = k;
}

void FCStructuredMesh::getIJKDim(int* ijk)
{
    if (!ijk) return;
    for (int i = 0; i < 3; ++i) ijk[i] = _ijkDim[i];
}

FCModelEnum::AbsModelType FCStructuredMesh::getAbsModelType()
{
    return FCModelEnum::AMTStructuredMesh;
}

void FCStructuredMesh::update()
{}

FCModelEnum::FITKMeshDim FCStructuredMesh::getMeshDim()
{
    int oneNum = 0;
    for (int i = 0; i < 3; ++i) {
        if (_ijkDim[i] == 1) oneNum++;
    }
    switch (oneNum) {
    case 0: return FCModelEnum::FMDimD3;
    case 1: return FCModelEnum::FMDimD2;
    case 2: return FCModelEnum::FMDimD1;
    case 3: return FCModelEnum::FMDimD0;
    default: break;
    }
    return FCModelEnum::FMDimNone;
}

} // namespace FC
