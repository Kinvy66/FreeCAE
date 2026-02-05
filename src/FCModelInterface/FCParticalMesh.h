/**
 * @file FCParticalMesh.h
 * @brief 粒子网格（移植自 FITKInterfaceModel）
 */
#ifndef FCPARTICALMESH_H
#define FCPARTICALMESH_H

#include "FCModelInterfaceAPI.h"
#include "FCAbstractMesh.h"
#include "FCNodeList.h"

namespace FC {

class FCMODELINTERFACE_API FCParticalMesh : public FCAbstractMesh, public FCNodeList
{
public:
    explicit FCParticalMesh() = default;
    ~FCParticalMesh() override = default;

    FCModelEnum::AbsModelType getAbsModelType() override;
    void update() override;
    FCModelEnum::FITKMeshDim getMeshDim() override;
};

} // namespace FC

#endif // FCPARTICALMESH_H
