/**
 * @file FCDataRepo.h
 * @brief 全局数据仓库（替代 FITKDataRepo，不依赖 FITKCore）
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#ifndef FCDATAREPO_H
#define FCDATAREPO_H

#include "FCDataAPI.h"
#include "FCType.h"
#include "FCAbstractDataObject.h"
#include "FCAbstractNamedDataObject.h"
#include "FCAbstractDataManager.hpp"
#include <QList>
#include <QMutex>
#include <QMutexLocker>

namespace FC {

class FCAbstractDataObject;

/**
 * @brief 数据仓库内部管理类，继承 FCAbstractDataManager<FCAbstractDataObject>，用列表维护全部数据对象（移植自 FITKDataRepoPrivate）
 * @details 仅从列表中移除对象时不释放内存（removeFromListOnly），供 FCDataRepo 在对象析构时调用
 */
class FCDATA_API FCDataRepoPrivate : public FCAbstractDataManager<FCAbstractDataObject>
{
public:
    explicit FCDataRepoPrivate() = default;
    ~FCDataRepoPrivate() override = default;

    void appendDataObj(FCAbstractDataObject* item) override;
    /** 只从列表中移除对象，不释放内存（供 FCDataRepo::removeDataObj 在对象析构时调用） */
    void removeFromListOnly(FCAbstractDataObject* obj);
};

class FCDATA_API FCDataRepo
{
public:
    static FCDataRepo* instance();

    void addDataObj(FCAbstractDataObject* obj);
    void removeDataObj(FCAbstractDataObject* obj);
    FCAbstractDataObject* getDataByID(FCID id) const;

    /**
     * @brief 按类型获取数据对象
     * @param id
     * @return 
     */
    template<typename T>
    T* getDataAs(FCID id) const { return dynamic_cast<T*>(getDataByID(id)); }


    /**
     * @brief 按类型获取第一个匹配的数据对象（用于获取全局唯一的 FCGeoCommandList 等）
     * @return 
     */
    template<typename T>
    T* getFirstDataByType() const
    {
        QMutexLocker locker(&mMutex);
        const int n = mRepoPrivate.getDataCount();
        for (int i = 0; i < n; ++i) {
            FCAbstractDataObject* obj = mRepoPrivate.getDataByIndex(i);
            T* t = dynamic_cast<T*>(obj);
            if (t) return t;
        }
        return nullptr;
    }

    /**
     * @brief 获取一个未被占用的有效 ID（在创建 FCAbstractDataObject 前调用）
     * @return 
     */
    FCID getNextValidID();

    /**
     * @brief 获取仓库中数据对象数量
     * @return 
     */
    int getDataCount() const;
    
    /**
     * @brief 按类型获取全部数据对象
     * @return 
     */
    template<typename T>
    QList<T*> getData() const
    {
        QMutexLocker locker(&mMutex);
        QList<T*> result;
        const int n = mRepoPrivate.getDataCount();
        for (int i = 0; i < n; ++i) {
            FCAbstractDataObject* obj = mRepoPrivate.getDataByIndex(i);
            T* t = dynamic_cast<T*>(obj);
            if (t) result.append(t);
        }
        return result;
    }

    /**
     * @brief 根据名称和类型获取数据 ID
     * @param name
     * @param compSens
     * @return 
     */
    template<typename T>
    FCID getTDataID(const QString& name, bool compSens = false) const
    {
        QMutexLocker locker(&mMutex);
        QList<FCAbstractDataObject*> data = mRepoPrivate.getDataObjectsByName(name, compSens);
        for (FCAbstractDataObject* d : data) {
            T* t = dynamic_cast<T*>(d);
            if (t) return d->getDataObjectID();
        }
        return FCID_INVALID;
    }
    
    /**
     * @brief 获取全部数据对象列表
     * @return 
     */
    QList<FCAbstractDataObject*> getDataList() const;

    
    /**
     * @brief 清空并重置仓库，仅保留 save 中的 ID
     * @param save
     */
    void resetRepo(QList<FCID> save = { FCID_INVALID });

private:
    FCDataRepo() = default;
    ~FCDataRepo() = default;
    FCDataRepo(const FCDataRepo&) = delete;
    FCDataRepo& operator=(const FCDataRepo&) = delete;

    FCDataRepoPrivate mRepoPrivate;
    static QMutex mMutex;
};

} // namespace FC

#define FCDATAREPO FC::FCDataRepo::instance()

#endif // FCDATAREPO_H
