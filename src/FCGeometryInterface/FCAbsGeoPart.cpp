/**
 * @file FCAbsGeoPart.cpp
 * @brief 部件命令实现（移植自 FITKInterfaceGeometry）
 */
#include "FCAbsGeoPart.h"
#include <QMutexLocker>

namespace FC {

FCGeoEnum::FCGeometryComType FCAbsGeoPart::getGeometryCommandType()
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
    QMutexLocker locker(&mMutex);
    FCAbsGeoCommand* subCmd = dynamic_cast<FCAbsGeoCommand*>(obj);
    if (!subCmd) return;
    subCmd->removeFromPart(this);
    if (mHelper) mHelper->removeDataObj(subCmd);
}

void FCAbsGeoPart::removeDataObjWithoutRelease(FCAbstractDataObject* obj)
{
    QMutexLocker locker(&mMutex);
    FCAbsGeoCommand* subCmd = dynamic_cast<FCAbsGeoCommand*>(obj);
    if (!subCmd) return;
    subCmd->removeFromPart(this);
    if (mHelper) mHelper->removeDataObjWithoutRelease(obj);
}

void FCAbsGeoPart::removeDataByIndex(int index)
{
    QMutexLocker locker(&mMutex);
    FCAbsGeoCommand* subCmd = mHelper ? dynamic_cast<FCAbsGeoCommand*>(mHelper->getDataByIndex(index)) : nullptr;
    if (!subCmd) return;
    subCmd->removeFromPart(this);
    if (mHelper) mHelper->removeDataObj(subCmd);
}

void FCAbsGeoPart::removeDataByID(int id)
{
    QMutexLocker locker(&mMutex);
    FCAbsGeoCommand* subCmd = mHelper ? dynamic_cast<FCAbsGeoCommand*>(mHelper->getDataByID(id)) : nullptr;
    if (!subCmd) return;
    subCmd->removeFromPart(this);
    if (mHelper) mHelper->removeDataObj(subCmd);
}

void FCAbsGeoPart::removeDataByName(const QString& name, bool compSens)
{
    QMutexLocker locker(&mMutex);
    FCAbsGeoCommand* subCmd = mHelper ? dynamic_cast<FCAbsGeoCommand*>(mHelper->getDataByName(name, compSens)) : nullptr;
    if (!subCmd) return;
    subCmd->removeFromPart(this);
    if (mHelper) mHelper->removeDataObj(subCmd);
}

} // namespace FC
