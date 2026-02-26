/**
 * @file FCDataRepo.cpp
 * @brief 全局数据仓库实现
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#include "FCDataRepo.h"
#include "FCAbstractDataObject.h"
#include <QMutex>

namespace FC {

// ----- FCDataRepoPrivate -----
void FCDataRepoPrivate::appendDataObj(FCAbstractDataObject* item)
{
    FCAbstractDataManager<FCAbstractDataObject>::appendDataObj(item);
}

void FCDataRepoPrivate::removeFromListOnly(FCAbstractDataObject* obj)
{
    if (!obj) return;
    while (mDataList.contains(obj))
        mDataList.removeOne(obj);
}

// ----- FCDataRepo -----
int FCDataRepo::mNextId = 1;
QMutex FCDataRepo::mMutex;

FCDataRepo* FCDataRepo::instance()
{
    static FCDataRepo s_instance;
    return &s_instance;
}

void FCDataRepo::addDataObj(FCAbstractDataObject* obj)
{
    if (!obj) return;
    QMutexLocker locker(&mMutex);
    int id = obj->getDataObjectID();
    if (id >= mNextId)
        mNextId = id + 1;
    mRepoPrivate.appendDataObj(obj);
}

void FCDataRepo::removeDataObj(FCAbstractDataObject* obj)
{
    if (!obj) return;
    QMutexLocker locker(&mMutex);
    mRepoPrivate.removeFromListOnly(obj);
}

FCAbstractDataObject* FCDataRepo::getDataByID(int id) const
{
    QMutexLocker locker(&mMutex);
    return mRepoPrivate.getDataByID(id);
}

int FCDataRepo::getNextValidID()
{
    QMutexLocker locker(&mMutex);
    int id = mNextId++;
    return id;
}

int FCDataRepo::getDataCount() const
{
    QMutexLocker locker(&mMutex);
    return mRepoPrivate.getDataCount();
}

QList<FCAbstractDataObject*> FCDataRepo::getDataList() const
{
    QMutexLocker locker(&mMutex);
    QList<FCAbstractDataObject*> list;
    const int n = mRepoPrivate.getDataCount();
    for (int i = 0; i < n; ++i)
        list.append(mRepoPrivate.getDataByIndex(i));
    return list;
}

void FCDataRepo::resetRepo(QList<int> save)
{
    QMutexLocker locker(&mMutex);
    int maxId = 0;
    QList<FCAbstractDataObject*> toRemove;
    const int n = mRepoPrivate.getDataCount();
    for (int i = 0; i < n; ++i) {
        FCAbstractDataObject* obj = mRepoPrivate.getDataByIndex(i);
        if (!obj) continue;
        int id = obj->getDataObjectID();
        if (save.contains(id)) {
            if (id > maxId) maxId = id;
        } else {
            toRemove.append(obj);
        }
    }
    for (FCAbstractDataObject* obj : toRemove)
        mRepoPrivate.removeDataObj(obj);
    mNextId = maxId + 1;
}

} // namespace FC
