/**
 * @file FCAbstractDataObject.cpp
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#include "FCAbstractDataObject.h"
#include "FCDataRepo.h"
#include <QMutex>
#include <QMutexLocker>

namespace FC {
static QMutex MUTEX;

FCAbstractDataObject::FCAbstractDataObject(FCID parentDataID)
{
    QMutexLocker locker(&MUTEX);
    
    mAbsDataID = FCDataRepo::instance()->getNextValidID();
    mParentDataID = parentDataID;
    FCDataRepo::instance()->addDataObj(this);
}

FCAbstractDataObject::~FCAbstractDataObject()
{
    emit dataObjectDestoried(this);
    FCDataRepo::instance()->removeDataObj(this);
}

FCID FCAbstractDataObject::getDataObjectID() const
{
    return mAbsDataID;
}

void FCAbstractDataObject::setParentDataID(FCID id)
{
    mParentDataID = id;
}

FCID FCAbstractDataObject::getParentDataID() const
{
    return mParentDataID;
}

QString FCAbstractDataObject::serialize(int)
{
    return QString();
}

bool FCAbstractDataObject::deserialize(const QString&, int)
{
    return false;
}

QVariant FCAbstractDataObject::getUserData(int userData) const
{
    return mUserData.value(userData);
}

void FCAbstractDataObject::setUserData(int ud, const QVariant& va)
{
    mUserData.insert(ud, va);
}

QList<int> FCAbstractDataObject::getUserDataKeys() const
{
    QList<int> keys;
    for (auto it = mUserData.begin(); it != mUserData.end(); ++it)
        keys.append(it.key());
    return keys;
}

bool FCAbstractDataObject::copy(FCAbstractDataObject* obj)
{
    if (!obj) return false;
    mParentDataID = obj->getParentDataID();
    for (int k : obj->getUserDataKeys())
        mUserData.insert(k, obj->getUserData(k));
    return true;
}

FCAbstractDataObject* FCAbstractDataObject::getParentObject()
{
    return FCDATAREPO->getDataByID(mParentDataID);
}

bool FCAbstractDataObject::isUsedDataObject(const QList<FCID>&)
{
    return false;
}

void FCAbstractDataObject::setAbsDataID(FCID id)
{
    mAbsDataID = id;
}

} // namespace FC
