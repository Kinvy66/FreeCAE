/**
 * @file FCMeshSurfaceNode.h
 * @brief 网格节点表面类（移植自 FITKInterfaceModel）
 */
#ifndef FCMESHSURFACENODE_H
#define FCMESHSURFACENODE_H

#include "FCModelInterfaceAPI.h"
#include "FCMeshSurface.h"

namespace FC {

class FCAbstractModel;
class FCModelSet;

class FCMODELINTERFACE_API FCMeshSurfaceNode : public FCMeshSurface
{
    FC_CLASS(FC, FCMeshSurfaceNode);
public:
    explicit FCMeshSurfaceNode() = default;
    virtual ~FCMeshSurfaceNode() = default;

    FCMeshSurface::MeshSurfaceType getMeshSurfaceType() override;
    FCModelSurface::ModelSurfaceType getModelSurfaceType() override;

    void addMeshSet(int modelID, int setID, double value);
    void addMeshSet(FCAbstractModel* model, FCModelSet* set, double value);
    void clearMeshSet();

    double getValueAt(int index) const;

    QList<SurfaceAbsSetInfo> getAbsoluteSetInfo() override;

protected:
    QList<double> _values;
};

} // namespace FC

#endif // FCMESHSURFACENODE_H
