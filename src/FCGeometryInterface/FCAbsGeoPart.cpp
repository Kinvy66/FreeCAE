/**
 * @file FCAbsGeoPart.cpp
 * @brief 部件命令实现（移植自 FITKInterfaceGeometry）
 */
#include "FCAbsGeoPart.h"
#include <QMutexLocker>

namespace FC {

FCGeoEnum::FITKGeometryComType FCAbsGeoPart::getGeometryCommandType()
{
    return FCGeoEnum::FGTPart;
}

bool FCAbsGeoPart::backtrackingShape()
{
    return false;
}

void FCAbsGeoPart::setAutoMergeSolid(bool flag)
{
    _autoMergeSolid = flag;
}

bool FCAbsGeoPart::getAutoMergeSolid()
{
    return _autoMergeSolid;
}

bool FCAbsGeoPart::update()
{
    return true;
}

void FCAbsGeoPart::removeDataObj(FCAbstractDataObject* obj)
{
    QMutexLocker locker(&_mutex);
    FCAbsGeoCommand* subCmd = dynamic_cast<FCAbsGeoCommand*>(obj);
    if (!subCmd) return;
    subCmd->removeFromPart(this);
    if (_helper) _helper->removeDataObj(subCmd);
}

void FCAbsGeoPart::removeDataObjWithoutRelease(FCAbstractDataObject* obj)
{
    QMutexLocker locker(&_mutex);
    FCAbsGeoCommand* subCmd = dynamic_cast<FCAbsGeoCommand*>(obj);
    if (!subCmd) return;
    subCmd->removeFromPart(this);
    if (_helper) _helper->removeDataObjWithoutRelease(obj);
}

void FCAbsGeoPart::removeDataByIndex(int index)
{
    QMutexLocker locker(&_mutex);
    FCAbsGeoCommand* subCmd = _helper ? dynamic_cast<FCAbsGeoCommand*>(_helper->getDataByIndex(index)) : nullptr;
    if (!subCmd) return;
    subCmd->removeFromPart(this);
    if (_helper) _helper->removeDataObj(subCmd);
}

void FCAbsGeoPart::removeDataByID(int id)
{
    QMutexLocker locker(&_mutex);
    FCAbsGeoCommand* subCmd = _helper ? dynamic_cast<FCAbsGeoCommand*>(_helper->getDataByID(id)) : nullptr;
    if (!subCmd) return;
    subCmd->removeFromPart(this);
    if (_helper) _helper->removeDataObj(subCmd);
}

void FCAbsGeoPart::removeDataByName(const QString& name, bool compSens)
{
    QMutexLocker locker(&_mutex);
    FCAbsGeoCommand* subCmd = _helper ? dynamic_cast<FCAbsGeoCommand*>(_helper->getDataByName(name, compSens)) : nullptr;
    if (!subCmd) return;
    subCmd->removeFromPart(this);
    if (_helper) _helper->removeDataObj(subCmd);
}

} // namespace FC
