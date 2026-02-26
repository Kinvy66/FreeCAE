/**
 * @file FCAbstractDataManager.hpp
 * @brief 数据管理器模板（替代 FITKAbstractDataManager，不依赖 FITKCore）
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#ifndef FCABSTRACTDATAMANAGER_HPP
#define FCABSTRACTDATAMANAGER_HPP

#include "FCAbstractDataManagerPrivate.h"
#include "FCAbstractDataManagerHelper.h"
#include "FCAbstractDataObject.h"
#include <QMutex>
#include <QMutexLocker>
#include <QList>
#include <QListIterator>

namespace FC {

template <class T>
class FCAbstractDataManager : public FCAbstractDataManagerPrivate
{
public:
    FCAbstractDataManager() { mHelper = new FCAbstractDataManagerHelper(mDataList); }
    ~FCAbstractDataManager() override { clear(); delete mHelper; mHelper = nullptr; }

    /**
     * @brief 追加对象
     * @param[i] item  追加的数据对象
     */
    void appendDataObj(FCAbstractDataObject* item) override {
        QMutexLocker locker(&mMutex);
        if (!isT(item)) delete item; else mHelper->appendData(item);
    }

    /**
     * @brief 批量追加对象
     * @param[i] items  追加的数据对象
     */
    void appendDataObj(const QList<FCAbstractDataObject*>& items) override {
        QMutexLocker locker(&mMutex); mHelper->appendData(items);
    }

    /**
     * @brief 获取管理的数据数量
     * @return int 数据数量
     */
    int getDataCount() const override { QMutexLocker locker(&mMutex); return mDataList.size(); }

    /**
     * @brief 是否包含数据对象
     * @param[i] obj        待判定对象指针
     * @return true   包含该对象
     * @return false  不包含该对象
     */
    bool isContains(FCAbstractDataObject* obj) const override { QMutexLocker locker(&mMutex); return mHelper && mHelper->isContains(obj); }
    void clear() override { QMutexLocker locker(&mMutex); if (mHelper) mHelper->clear(); }

    /**
     * @brief 插入数据对象
     * @param[i] index         插入的索引
     * @param[i] item          插入的数据对象
     */
    void insertDataObj(int index, FCAbstractDataObject* item) override {
        QMutexLocker locker(&mMutex); if (!isT(item)) delete item; else mHelper->insertDataObj(index, item);
    }

    /**
     * @brief 获取数据对象的索引
     * @param[i] obj        待获取索引的数据对象
     * @return int 数据对象的索引
     */
    int getDataIndex(FCAbstractDataObject* obj) const override { QMutexLocker locker(&mMutex); return mHelper ? mHelper->getDataIndex(obj) : -1; }

    /**
     * @brief 获取数据对象的名称
     * @param[i] name        数据对象的名称
     * @param[i] compSens    是否区分大小写
     * @return FCAbstractDataObject* 数据对象
     */
    FCAbstractDataObject* getDataObjectByName(const QString& name, bool compSens = false) override {
        QMutexLocker locker(&mMutex); return mHelper ? mHelper->getDataByName(name, compSens) : nullptr;
    }

    /**
     * @brief 获取数据对象的名称列表
     * @param[i] name        数据对象的名称
     * @param[i] compSens    是否区分大小写
     * @return QList<FCAbstractDataObject*> 数据对象列表
     */
    QList<FCAbstractDataObject*> getDataObjectsByName(const QString& name, bool compSens = false) override {
        QMutexLocker locker(&mMutex); return mHelper ? mHelper->getDataListByName(name, compSens) : QList<FCAbstractDataObject*>();
    }

    /**
     * @brief 删除数据对象
     * @param[i] obj        待删除的数据对象
     */
    void removeDataObj(FCAbstractDataObject* obj) override { QMutexLocker locker(&mMutex); if (mHelper) mHelper->removeDataObj(obj); }

    /**
     * @brief 删除数据对象，不释放对象
     * @param[i] obj        待删除的数据对象
     */
    void removeDataObjWithoutRelease(FCAbstractDataObject* obj) override { QMutexLocker locker(&mMutex); if (mHelper) mHelper->removeDataObjWithoutRelease(obj); }

    /**
     * @brief 删除数据对象
     * @param[i] index         删除的索引
     */
    void removeDataByIndex(int index) override { QMutexLocker locker(&mMutex); if (mHelper) mHelper->removeDataByIndex(index); }

    /**
     * @brief 删除数据对象
     * @param[i] id        删除的数据对象ID
     */
    void removeDataByID(int id) override { QMutexLocker locker(&mMutex); if (mHelper) mHelper->removeDataByID(id); }

    /**
     * @brief 删除数据对象
     * @param[i] name        删除的数据对象名称
     * @param[i] compSens    是否区分大小写
     */
    void removeDataByName(const QString& name, bool compSens = false) override { QMutexLocker locker(&mMutex); if (mHelper) mHelper->removeDataByName(name, compSens); }

    /**
     * @brief 检查数据对象名称
     * @param[i] name        检查的数据对象名称
     * @return QString 检查后的数据对象名称
     */
    QString checkName(const QString& name) override { QMutexLocker locker(&mMutex); return mHelper ? mHelper->checkName(name) : name; }

    /**
     * @brief 获取数据对象的索引
     * @param[i] dataId        获取的数据对象ID
     * @return int 数据对象的索引
     */
    int getIndexByID(int dataId) override { QMutexLocker locker(&mMutex); return mHelper ? mHelper->getIndexByID(dataId) : -1; }

    /**
     * @brief 获取数据对象的ID
     * @param[i] dataIndex        获取的数据对象索引
     * @return int 数据对象的ID
     */
    int getIDByIndex(int dataIndex) override { QMutexLocker locker(&mMutex); return mHelper ? mHelper->getIDByIndex(dataIndex) : -1; }

    /**
     * @brief 排序数据对象
     * @param[i] type        排序类型
     */
    void sortObject(ObjectSortType type) override { QMutexLocker locker(&mMutex); if (mHelper) mHelper->sortObject(type); }

    /**
     * @brief 排序数据对象
     * @param[i] function        排序函数
     */
    void sortObject(std::function<bool(FCAbstractDataObject*, FCAbstractDataObject*)> function) override { QMutexLocker locker(&mMutex); if (mHelper) mHelper->sortObject(function); }

    /**
     * @brief 判断数据对象是否被使用
     * @param[i] data        数据对象ID列表
     * @return bool 是否被使用
     */
    bool isUsedDataObject(const QList<int>& data) override { QMutexLocker locker(&mMutex); return mHelper ? mHelper->isUsedDataObject(data) : false; }

    /**
     * @brief 获取数据对象
     * @param[i] index        数据对象索引
     * @return T* 数据对象
     */
    T* getDataByIndex(int index) { QMutexLocker locker(&mMutex); return toT(mHelper ? mHelper->getDataByIndex(index) : nullptr); }
    T* getDataByIndex(int index) const { QMutexLocker locker(&mMutex); return toT(mHelper ? mHelper->getDataByIndex(index) : nullptr); }

    /**
     * @brief 获取数据对象
     * @param[i] id        数据对象ID
     * @return T* 数据对象
     */
    T* getDataByID(int id) { QMutexLocker locker(&mMutex); return toT(mHelper ? mHelper->getDataByID(id) : nullptr); }
    T* getDataByID(int id) const { QMutexLocker locker(&mMutex); return toT(mHelper ? mHelper->getDataByID(id) : nullptr); }

    /**
     * @brief 获取数据对象
     * @param[i] name        数据对象名称
     * @param[i] compSens    是否区分大小写
     * @return T* 数据对象
     */
    T* getDataByName(const QString& name, bool compSens = false) { QMutexLocker locker(&mMutex); return toT(mHelper ? mHelper->getDataByName(name, compSens) : nullptr); }

    /**
     * @brief 获取数据对象
     * @param[i] index        数据对象索引
     * @return T* 数据对象
     */
    T* operator[](int index) { return getDataByIndex(index); }

    /**
     * @brief 设置父数据对象ID
     * @param[i] id        父数据对象ID
     */
    void setParentDataIDM(int id) { if (mHelper) mHelper->setParentDataID(id); }

    /**
     * @brief 获取父数据对象ID
     * @return int 父数据对象ID
     */
    int getMParentDataIDM() const { return mHelper ? mHelper->getParentDataID() : -1; }

private:
    /**
     * @brief 判断数据对象是否为T类型
     * @param[i] obj        数据对象
     * @return bool 是否为T类型
     */
    bool isT(FCAbstractDataObject* obj) const { return dynamic_cast<T*>(obj) != nullptr; }

    /**
     * @brief 将数据对象转换为T类型
     * @param[i] obj        数据对象
     * @return T* 转换后的数据对象
     */
    T* toT(FCAbstractDataObject* obj) const { return dynamic_cast<T*>(obj); }

protected:
    FCAbstractDataManagerHelper* mHelper{};
    mutable QMutex mMutex;
    QList<FCAbstractDataObject*> mDataList;
};

} // namespace FC

#endif // FCABSTRACTDATAMANAGER_HPP
