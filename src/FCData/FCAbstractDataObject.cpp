/**
 * @file FCAbstractDataObject.cpp
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#include "FCAbstractDataObject.h"
#include "FCDataRepo.h"

namespace FC {

FCAbstractDataObject::FCAbstractDataObject(int parentDataID)
{
    _absDataID = FCDataRepo::instance()->getNextValidID();
    _parentDataID = parentDataID;
    FCDataRepo::instance()->addDataObj(this);
}

FCAbstractDataObject::~FCAbstractDataObject()
{
    emit dataObjectDestoried(this);
    FCDataRepo::instance()->removeDataObj(this);
}

int FCAbstractDataObject::getDataObjectID() const
{
    return _absDataID;
}

void FCAbstractDataObject::setParentDataID(int id)
{
    _parentDataID = id;
}

int FCAbstractDataObject::getParentDataID() const
{
    return _parentDataID;
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
    return _userData.value(userData);
}

void FCAbstractDataObject::setUserData(int ud, const QVariant& va)
{
    _userData.insert(ud, va);
}

QList<int> FCAbstractDataObject::getUserDataKeys() const
{
    QList<int> keys;
    for (auto it = _userData.begin(); it != _userData.end(); ++it)
        keys.append(it.key());
    return keys;
}

bool FCAbstractDataObject::copy(FCAbstractDataObject* obj)
{
    if (!obj) return false;
    _parentDataID = obj->getParentDataID();
    for (int k : obj->getUserDataKeys())
        _userData.insert(k, obj->getUserData(k));
    return true;
}

FCAbstractDataObject* FCAbstractDataObject::getParentObject()
{
    return FCDATAREPO->getDataByID(_parentDataID);
}

bool FCAbstractDataObject::isUsedDataObject(const QList<int>&)
{
    return false;
}

void FCAbstractDataObject::setAbsDataID(int id)
{
    _absDataID = id;
}

} // namespace FC
