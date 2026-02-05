/**
 * @file FCComponentManager.h
 * @brief 集合/组件管理器（移植自 FITKInterfaceModel）
 */
#ifndef FCCOMPONENTMANAGER_H
#define FCCOMPONENTMANAGER_H

#include "FCModelInterfaceAPI.h"
#include "FCAbstractModelComponent.h"
#include "FCModelSet.h"
#include "FCMeshSurface.h"
#include "FCModelSurface.h"
#include "FCModelEnum.h"
#include <FCData/FCAbstractDataObject.h>
#include <FCData/FCAbstractDataManager.hpp>
#include <QList>

namespace FC {

class FCMODELINTERFACE_API FCComponentManager : public FCAbstractDataObject,
    public FCAbstractDataManager<FCAbstractModelComponent>
{
public:
    explicit FCComponentManager() = default;
    virtual ~FCComponentManager() = default;

    QList<FCModelSet*> getModelSet(FCModelEnum::FITKModelSetType t);
    QList<FCModelSet*> getUnInternalModelSet(FCModelEnum::FITKModelSetType t);
    QList<FCMeshSurface*> getAllMeshSurface();
    QList<FCMeshSurface*> getMeshSurface(FCMeshSurface::MeshSurfaceType t);
    QList<FCModelSurface*> getModelSurface(FCModelSurface::ModelSurfaceType t);
    QList<FCMeshSurface*> getUnInternalMeshSurface(FCMeshSurface::MeshSurfaceType t);
    bool isModelSet(int index);
    FCModelSet* getAsModelSet(int index);
    FCMeshSurface* getAsMeshSurf(int index);
    FCModelSurface* getSurf(int index);
    QList<FCModelSet*> getAllSet(bool recur = true);
    QList<FCModelSurface*> getAllSurface();

    void appendDataObj(FCAbstractDataObject* item) override;
    void insertDataObj(int index, FCAbstractDataObject* item) override;
    FCAbstractModelComponent* getDataByID(int id);

    virtual FCModelSet* getModelSetByMember(FCModelEnum::FITKModelSetType t,
        int modelID, const QList<int>& member);
    FCModelSet* getSingleNodeSet(int modelID, int memID);

private:
    void mergeSameNameSet(FCModelSet* existSet, FCModelSet* item);
};

} // namespace FC

#endif // FCCOMPONENTMANAGER_H
