/**
 * @file FCVirtualTopoManager.h
 * @brief 虚拓扑管理器（移植自 FITKInterfaceGeometry）
 */
#ifndef FCVIRTUALTOPOMANAGER_H
#define FCVIRTUALTOPOMANAGER_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsVirtualTopo.h"
#include "FCGeoEnum.h"
#include <FCData/FCAbstractDataManager.hpp>
#include <FCData/FCAbstractDataObject.h>
#include <QHash>
#include <QList>
#include <QMutex>

namespace FC {

class FCGEOMETRYINTERFACE_API FCShapeVirtualTopoManager
    : public FCAbstractDataManager<FCAbsVirtualTopo>
{
public:
    explicit FCShapeVirtualTopoManager() = default;
    ~FCShapeVirtualTopoManager() override = default;
};

class FCGEOMETRYINTERFACE_API FCVirtualTopoManager : public FCAbstractDataObject
{
public:
    explicit FCVirtualTopoManager();
    virtual ~FCVirtualTopoManager();

    void clear();
    void setRootObj(FCAbsVirtualTopo* topoObj);
    void addRootObj(FCAbsVirtualTopo* topoObj);
    FCAbsVirtualTopo* getRootObj(int index = 0);
    int getNumberOfRootObjs();
    void appendVirtualTopoObj(FCAbsVirtualTopo* tobj);
    void appendVirtualTopoObj(FCAbsVirtualTopo* tobj, FCGeoEnum::VTopoShapeType st);
    FCAbsVirtualTopo* getVirtualTopo(void* shape, FCGeoEnum::VTopoShapeType type);
    FCShapeVirtualTopoManager* getShapeVirtualTopoManager(FCGeoEnum::VTopoShapeType type);
    int getShapeVirtualTopoCount(FCGeoEnum::VTopoShapeType type);

private:
    QMutex _mutex;
    QList<int> _rootTopoObjs;
    QHash<FCGeoEnum::VTopoShapeType, FCShapeVirtualTopoManager*> _shapeVirtualTopos;
};

} // namespace FC

#endif // FCVIRTUALTOPOMANAGER_H
