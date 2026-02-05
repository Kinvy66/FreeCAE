/**
 * @file FCDataRepo.cpp
 * @brief 全局数据仓库实现
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#include "FCDataRepo.h"
#include "FCAbstractDataObject.h"
#include <QMutex>

namespace FC {

int FCDataRepo::_nextId = 1;

FCDataRepo* FCDataRepo::instance()
{
    static FCDataRepo s_instance;
    return &s_instance;
}

void FCDataRepo::addDataObj(FCAbstractDataObject* obj)
{
    if (!obj) return;
    QMutexLocker locker(&_mutex);
    _dataMap.insert(obj->getDataObjectID(), obj);
}

void FCDataRepo::removeDataObj(FCAbstractDataObject* obj)
{
    if (!obj) return;
    QMutexLocker locker(&_mutex);
    _dataMap.remove(obj->getDataObjectID());
}

FCAbstractDataObject* FCDataRepo::getDataByID(int id) const
{
    QMutexLocker locker(&_mutex);
    return _dataMap.value(id, nullptr);
}

int FCDataRepo::getNextValidID()
{
    QMutexLocker locker(&_mutex);
    int id = _nextId++;
    return id;
}

} // namespace FC
