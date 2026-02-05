/**
 * @file FCVirtualTopoManager.cpp
 * @brief 虚拓扑管理器实现（移植自 FITKInterfaceGeometry）
 */
#include "FCVirtualTopoManager.h"
#include <QMutexLocker>
#include <QtAlgorithms>

namespace FC {

FCVirtualTopoManager::FCVirtualTopoManager()
    : FCAbstractDataObject(-1)
{
    _shapeVirtualTopos.insert(FCGeoEnum::VSPoint, new FCShapeVirtualTopoManager);
    _shapeVirtualTopos.insert(FCGeoEnum::VSEdge, new FCShapeVirtualTopoManager);
    _shapeVirtualTopos.insert(FCGeoEnum::VSWire, new FCShapeVirtualTopoManager);
    _shapeVirtualTopos.insert(FCGeoEnum::VSFace, new FCShapeVirtualTopoManager);
    _shapeVirtualTopos.insert(FCGeoEnum::VSShell, new FCShapeVirtualTopoManager);
    _shapeVirtualTopos.insert(FCGeoEnum::VSSolid, new FCShapeVirtualTopoManager);
    _shapeVirtualTopos.insert(FCGeoEnum::VSLump, new FCShapeVirtualTopoManager);
    _shapeVirtualTopos.insert(FCGeoEnum::VSAssembly, new FCShapeVirtualTopoManager);
}

FCVirtualTopoManager::~FCVirtualTopoManager()
{
    clear();
    for (FCShapeVirtualTopoManager* sm : _shapeVirtualTopos) {
        if (sm) delete sm;
    }
    _shapeVirtualTopos.clear();
}

void FCVirtualTopoManager::clear()
{
    QMutexLocker locker(&_mutex);
    for (FCShapeVirtualTopoManager* sm : _shapeVirtualTopos) {
        if (sm) sm->clear();
    }
    _rootTopoObjs.clear();
}

void FCVirtualTopoManager::setRootObj(FCAbsVirtualTopo* topoObj)
{
    QMutexLocker locker(&_mutex);
    if (!topoObj) return;
    _rootTopoObjs.clear();
    _rootTopoObjs.push_back(topoObj->getDataObjectID());
}

void FCVirtualTopoManager::addRootObj(FCAbsVirtualTopo* topoObj)
{
    if (!topoObj) return;
    _rootTopoObjs.append(topoObj->getDataObjectID());
}

FCAbsVirtualTopo* FCVirtualTopoManager::getRootObj(int index)
{
    QMutexLocker locker(&_mutex);
    if (index < 0 || index >= _rootTopoObjs.size()) return nullptr;
    int rootId = _rootTopoObjs.at(index);
    QList<FCGeoEnum::VTopoShapeType> types = _shapeVirtualTopos.keys();
    std::sort(types.begin(), types.end());
    for (int i = types.size() - 1; i >= 0; --i) {
        FCShapeVirtualTopoManager* sm = _shapeVirtualTopos.value(types.at(i));
        if (!sm) continue;
        FCAbsVirtualTopo* topo = sm->getDataByID(rootId);
        if (topo) return topo;
    }
    return nullptr;
}

int FCVirtualTopoManager::getNumberOfRootObjs()
{
    return _rootTopoObjs.size();
}

void FCVirtualTopoManager::appendVirtualTopoObj(FCAbsVirtualTopo* tobj)
{
    QMutexLocker locker(&_mutex);
    if (!tobj) return;
    FCGeoEnum::VTopoShapeType t = tobj->getShapeType();
    FCShapeVirtualTopoManager* mgr = _shapeVirtualTopos.value(t);
    if (!mgr) {
        mgr = new FCShapeVirtualTopoManager;
        _shapeVirtualTopos[t] = mgr;
    }
    mgr->appendDataObj(tobj);
}

void FCVirtualTopoManager::appendVirtualTopoObj(FCAbsVirtualTopo* tobj, FCGeoEnum::VTopoShapeType st)
{
    QMutexLocker locker(&_mutex);
    if (!tobj) return;
    FCShapeVirtualTopoManager* mgr = _shapeVirtualTopos.value(st);
    if (!mgr) {
        mgr = new FCShapeVirtualTopoManager;
        _shapeVirtualTopos[st] = mgr;
    }
    mgr->appendDataObj(tobj);
}

FCAbsVirtualTopo* FCVirtualTopoManager::getVirtualTopo(void* shape, FCGeoEnum::VTopoShapeType type)
{
    Q_UNUSED(shape);
    FCShapeVirtualTopoManager* mgr = getShapeVirtualTopoManager(type);
    if (!mgr) return nullptr;
    const int n = mgr->getDataCount();
    for (int i = 0; i < n; ++i) {
        FCAbsVirtualTopo* topo = mgr->getDataByIndex(i);
        if (topo && topo->getShape() && topo->getShape()->isSameShape(shape))
            return topo;
    }
    return nullptr;
}

FCShapeVirtualTopoManager* FCVirtualTopoManager::getShapeVirtualTopoManager(FCGeoEnum::VTopoShapeType type)
{
    return _shapeVirtualTopos.value(type, nullptr);
}

int FCVirtualTopoManager::getShapeVirtualTopoCount(FCGeoEnum::VTopoShapeType type)
{
    FCShapeVirtualTopoManager* mgr = getShapeVirtualTopoManager(type);
    return mgr ? mgr->getDataCount() : 0;
}

} // namespace FC
