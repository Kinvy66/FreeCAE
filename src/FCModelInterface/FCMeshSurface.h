/**
 * @file FCMeshSurface.h
 * @brief 网格表面类（移植自 FITKInterfaceModel）
 */
#ifndef FCMESHSURFACE_H
#define FCMESHSURFACE_H

#include "FCModelInterfaceAPI.h"
#include "FCModelSurface.h"
#include <FCData/FCMacros.h>

namespace FC {

class FCModelSet;

class FCMODELINTERFACE_API FCMeshSurface : public FCModelSurface
{
    Q_OBJECT
    FC_CLASS(FC, FCMeshSurface);
public:
    enum MeshSurfaceType
    {
        SurNone,
        SurEle,
        SurNode,
    };
    Q_ENUM(MeshSurfaceType);

    explicit FCMeshSurface() = default;
    virtual ~FCMeshSurface() = 0;

    virtual MeshSurfaceType getMeshSurfaceType() = 0;
    int getMeshSetCount() const;
    FCModelSet* getMeshSetAt(int index) const;
};

} // namespace FC

#endif // FCMESHSURFACE_H
