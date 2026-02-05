/**
 * @file FCUnstructuredMesh.h
 * @brief 非结构网格（移植自 FITKInterfaceModel）
 */
#ifndef FCUNSTRUCTUREDMESH_H
#define FCUNSTRUCTUREDMESH_H

#include "FCModelInterfaceAPI.h"
#include "FCAbstractMesh.h"
#include "FCNodeList.h"
#include "FCElementList.h"
#include "FCModelEnum.h"

namespace FC {

class FCUnstructuredMeshTopo;

class FCMODELINTERFACE_API FCUnstructuredMesh : public FCAbstractMesh, public FCNodeList, public FCElementList
{
public:
    explicit FCUnstructuredMesh();
    virtual ~FCUnstructuredMesh();

    virtual FCModelEnum::AbsModelType getAbsModelType() override;
    virtual void update() override;
    virtual FCModelEnum::FITKMeshDim getMeshDim() override;
    unsigned int getMeshDimBit() override;

    FCElementQuality checkElementQuality(int eleIndex);
    bool getElementDirection(double* dir, int id);
    bool hasOrphanMesh();
    bool hasNativeMesh();
    virtual void clearNativeMesh();
    virtual void clearOrphanMesh();
    virtual bool buildUnstructuredMeshTopo();
    FCUnstructuredMeshTopo* getUnstructuredMeshTopo();
    virtual bool getPointCoor(int pointID, double* coor, int modelIndex = 0) override;

private:
    double calMeshEdgeLength(const QList<int>& edge);
    bool getLineEleDirection(double* dir, FCAbstractElement* ele);
    bool getShellEleDirection(double* dir, FCAbstractElement* ele);

    FCUnstructuredMeshTopo* _meshTopo{ nullptr };
};

} // namespace FC

#endif // FCUNSTRUCTUREDMESH_H
