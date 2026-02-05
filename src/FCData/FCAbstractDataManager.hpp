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
    FCAbstractDataManager() { _helper = new FCAbstractDataManagerHelper(_dataList); }
    ~FCAbstractDataManager() override { clear(); delete _helper; _helper = nullptr; }

    void appendDataObj(FCAbstractDataObject* item) override {
        QMutexLocker locker(&_mutex);
        if (!isT(item)) delete item; else _helper->appendData(item);
    }
    void appendDataObj(const QList<FCAbstractDataObject*>& items) override {
        QMutexLocker locker(&_mutex); _helper->appendData(items);
    }
    int getDataCount() const override { QMutexLocker locker(&_mutex); return _dataList.size(); }
    bool isContains(FCAbstractDataObject* obj) const override { QMutexLocker locker(&_mutex); return _helper && _helper->isContains(obj); }
    void clear() override { QMutexLocker locker(&_mutex); if (_helper) _helper->clear(); }
    void insertDataObj(int index, FCAbstractDataObject* item) override {
        QMutexLocker locker(&_mutex); if (!isT(item)) delete item; else _helper->insertDataObj(index, item);
    }
    int getDataIndex(FCAbstractDataObject* obj) const override { QMutexLocker locker(&_mutex); return _helper ? _helper->getDataIndex(obj) : -1; }
    FCAbstractDataObject* getDataObjectByName(const QString& name, bool compSens = false) override {
        QMutexLocker locker(&_mutex); return _helper ? _helper->getDataByName(name, compSens) : nullptr;
    }
    QList<FCAbstractDataObject*> getDataObjectsByName(const QString& name, bool compSens = false) override {
        QMutexLocker locker(&_mutex); return _helper ? _helper->getDataListByName(name, compSens) : QList<FCAbstractDataObject*>();
    }
    void removeDataObj(FCAbstractDataObject* obj) override { QMutexLocker locker(&_mutex); if (_helper) _helper->removeDataObj(obj); }
    void removeDataObjWithoutRelease(FCAbstractDataObject* obj) override { QMutexLocker locker(&_mutex); if (_helper) _helper->removeDataObjWithoutRelease(obj); }
    void removeDataByIndex(int index) override { QMutexLocker locker(&_mutex); if (_helper) _helper->removeDataByIndex(index); }
    void removeDataByID(int id) override { QMutexLocker locker(&_mutex); if (_helper) _helper->removeDataByID(id); }
    void removeDataByName(const QString& name, bool compSens = false) override { QMutexLocker locker(&_mutex); if (_helper) _helper->removeDataByName(name, compSens); }
    QString checkName(const QString& name) override { QMutexLocker locker(&_mutex); return _helper ? _helper->checkName(name) : name; }
    int getIndexByID(int dataId) override { QMutexLocker locker(&_mutex); return _helper ? _helper->getIndexByID(dataId) : -1; }
    int getIDByIndex(int dataIndex) override { QMutexLocker locker(&_mutex); return _helper ? _helper->getIDByIndex(dataIndex) : -1; }
    void sortObject(ObjectSortType type) override { QMutexLocker locker(&_mutex); if (_helper) _helper->sortObject(type); }
    void sortObject(std::function<bool(FCAbstractDataObject*, FCAbstractDataObject*)> function) override { QMutexLocker locker(&_mutex); if (_helper) _helper->sortObject(function); }
    bool isUsedDataObject(const QList<int>& data) override { QMutexLocker locker(&_mutex); return _helper ? _helper->isUsedDataObject(data) : false; }

    T* getDataByIndex(int index) { QMutexLocker locker(&_mutex); return toT(_helper ? _helper->getDataByIndex(index) : nullptr); }
    T* getDataByID(int id) { QMutexLocker locker(&_mutex); return toT(_helper ? _helper->getDataByID(id) : nullptr); }
    T* getDataByName(const QString& name, bool compSens = false) { QMutexLocker locker(&_mutex); return toT(_helper ? _helper->getDataByName(name, compSens) : nullptr); }
    T* operator[](int index) { return getDataByIndex(index); }
    void setParentDataIDM(int id) { if (_helper) _helper->setParentDataID(id); }
    int getMParentDataIDM() const { return _helper ? _helper->getParentDataID() : -1; }

private:
    bool isT(FCAbstractDataObject* obj) const { return dynamic_cast<T*>(obj) != nullptr; }
    T* toT(FCAbstractDataObject* obj) const { return dynamic_cast<T*>(obj); }

protected:
    FCAbstractDataManagerHelper* _helper{};
    mutable QMutex _mutex;
    QList<FCAbstractDataObject*> _dataList;
};

} // namespace FC

#endif // FCABSTRACTDATAMANAGER_HPP
