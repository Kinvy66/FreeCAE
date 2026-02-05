/**
 * @file FCAbstractMesh.h
 * @brief 抽象网格模型定义（移植自 FITKInterfaceModel）
 */
#ifndef FCABSTRACTMESH_H
#define FCABSTRACTMESH_H

#include "FCModelInterfaceAPI.h"
#include "FCAbstractModel.h"

namespace FC {

class FCMODELINTERFACE_API FCAbstractMesh : public FCAbstractModel
{
public:
    explicit FCAbstractMesh() = default;
    virtual ~FCAbstractMesh() = 0;

    virtual FCModelEnum::AbsModelType getAbsModelType() override;
    virtual void update() = 0;
    virtual FCModelEnum::FITKMeshDim getMeshDim();
    virtual unsigned int getMeshDimBit();
};

} // namespace FC

#endif // FCABSTRACTMESH_H
