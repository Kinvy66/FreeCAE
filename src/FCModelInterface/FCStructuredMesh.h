/**
 * @file FCStructuredMesh.h
 * @brief 结构网格（移植自 FITKInterfaceModel）
 */
#ifndef FCSTRUCTUREDMESH_H
#define FCSTRUCTUREDMESH_H

#include "FCModelInterfaceAPI.h"
#include "FCAbstractMesh.h"
#include "FCNodeList.h"

namespace FC {

class FCMODELINTERFACE_API FCStructuredMesh : public FCAbstractMesh, public FCNodeList
{
public:
    explicit FCStructuredMesh() = default;
    ~FCStructuredMesh() override = default;

    void setIJKDim(int i, int j, int k);
    void getIJKDim(int* ijk);

    FCModelEnum::AbsModelType getAbsModelType() override;
    void update() override;
    FCModelEnum::FITKMeshDim getMeshDim() override;

private:
    int _ijkDim[3] = { 1, 1, 1 };
};

} // namespace FC

#endif // FCSTRUCTUREDMESH_H
