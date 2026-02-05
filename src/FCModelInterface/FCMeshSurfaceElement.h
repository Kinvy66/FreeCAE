/**
 * @file FCMeshSurfaceElement.h
 * @brief 网格单元表面类（移植自 FITKInterfaceModel）
 */
#ifndef FCMESHSURFACEELEMENT_H
#define FCMESHSURFACEELEMENT_H

#include "FCModelInterfaceAPI.h"
#include "FCMeshSurface.h"

namespace FC {

class FCAbstractModel;
class FCModelSet;

class FCMODELINTERFACE_API FCMeshSurfaceElement : public FCMeshSurface
{
    FC_CLASS(FC, FCMeshSurfaceElement);
public:
    explicit FCMeshSurfaceElement() = default;
    virtual ~FCMeshSurfaceElement() = default;

    FCMeshSurface::MeshSurfaceType getMeshSurfaceType() override;
    FCModelSurface::ModelSurfaceType getModelSurfaceType() override;

    void addMeshSet(int modelID, int setID, int surfIndex);
    void addMeshSet(FCAbstractModel* model, FCModelSet* set, int surfIndex);
    void clearMeshSet();

    int getSurfaceIndexAt(int index) const;
    void setSurfaceIndex(int index, int surfIndex);

    QList<SurfaceAbsSetInfo> getAbsoluteSetInfo() override;

protected:
    QList<int> _surfIndex;
};

} // namespace FC

#endif // FCMESHSURFACEELEMENT_H
