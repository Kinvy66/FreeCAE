/**
 * @file FCDataRepo.cpp
 * @brief 全局数据仓库实现
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#include "FCDataRepo.h"
#include "FCAbstractDataObject.h"
#include "FCUniqueIDGenerater.h"
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
    mRepoPrivate.appendDataObj(obj);
}

void FCDataRepo::removeDataObj(FCAbstractDataObject* obj)
{
    if (!obj) return;
    QMutexLocker locker(&mMutex);
    mRepoPrivate.removeFromListOnly(obj);
}

FCAbstractDataObject* FCDataRepo::getDataByID(FCID id) const
{
    QMutexLocker locker(&mMutex);
    return mRepoPrivate.getDataByID(id);
}

FCID FCDataRepo::getNextValidID()
{
    QMutexLocker locker(&mMutex);
    FCID id;
    do {
        id = static_cast<FCID>(FCUniqueIDGenerater::id_uint64());
    } while (mRepoPrivate.getDataByID(id) != nullptr);
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

void FCDataRepo::resetRepo(QList<FCID> save)
{
    QMutexLocker locker(&mMutex);
    QList<FCAbstractDataObject*> toRemove;
    const int n = mRepoPrivate.getDataCount();
    for (int i = 0; i < n; ++i) {
        FCAbstractDataObject* obj = mRepoPrivate.getDataByIndex(i);
        if (!obj) continue;
        FCID id = obj->getDataObjectID();
        if (!save.contains(id))
            toRemove.append(obj);
    }
    for (FCAbstractDataObject* obj : toRemove)
        mRepoPrivate.removeDataObj(obj);
}

} // namespace FC
