/**
 * @file FCDataRepo.h
 * @brief 全局数据仓库（替代 FITKDataRepo，不依赖 FITKCore）
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#ifndef FCDATAREPO_H
#define FCDATAREPO_H

#include "FCDataAPI.h"
#include <QHash>
#include <QMutex>
#include <QMutexLocker>

namespace FC {

class FCAbstractDataObject;

class FCDATA_API FCDataRepo
{
public:
    static FCDataRepo* instance();

    void addDataObj(FCAbstractDataObject* obj);
    void removeDataObj(FCAbstractDataObject* obj);
    FCAbstractDataObject* getDataByID(int id) const;

    /** 按类型获取数据对象 */
    template<typename T>
    T* getDataAs(int id) const { return dynamic_cast<T*>(getDataByID(id)); }

    /** 获取一个未被占用的有效 ID（在创建 FCAbstractDataObject 前调用） */
    int getNextValidID();

private:
    FCDataRepo() = default;
    ~FCDataRepo() = default;
    FCDataRepo(const FCDataRepo&) = delete;
    FCDataRepo& operator=(const FCDataRepo&) = delete;

    QHash<int, FCAbstractDataObject*> _dataMap;
    mutable QMutex _mutex;
    static int _nextId;
};

} // namespace FC

#define FCDATAREPO FC::FCDataRepo::instance()

#endif // FCDATAREPO_H
