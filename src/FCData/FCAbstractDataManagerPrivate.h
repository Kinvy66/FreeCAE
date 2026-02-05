/**
 * @file FCAbstractDataManagerPrivate.h
 * @brief 数据管理器抽象接口（替代 FITKAbstractDataManagerPrivate）
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#ifndef FCABSTRACTDATAMANAGERPRIVATE_H
#define FCABSTRACTDATAMANAGERPRIVATE_H

#include "FCDataAPI.h"
#include <QList>
#include <QString>
#include <functional>

namespace FC {

class FCAbstractDataObject;

enum class ObjectSortType { DataObjID, DataName };

class FCDATA_API FCAbstractDataManagerPrivate
{
public:
    explicit FCAbstractDataManagerPrivate() = default;
    virtual ~FCAbstractDataManagerPrivate() = 0;

    virtual void appendDataObj(FCAbstractDataObject* item) = 0;
    virtual void appendDataObj(const QList<FCAbstractDataObject*>& items) = 0;
    virtual int getDataCount() const = 0;
    virtual bool isContains(FCAbstractDataObject* obj) const = 0;
    virtual void clear() = 0;
    virtual void insertDataObj(int index, FCAbstractDataObject* item) = 0;
    virtual int getDataIndex(FCAbstractDataObject* obj) const = 0;
    virtual FCAbstractDataObject* getDataObjectByName(const QString& name, bool compSens = false) = 0;
    virtual QList<FCAbstractDataObject*> getDataObjectsByName(const QString& name, bool compSens = false) = 0;
    virtual void removeDataObj(FCAbstractDataObject* obj) = 0;
    virtual void removeDataObjWithoutRelease(FCAbstractDataObject* obj) = 0;
    virtual void removeDataByIndex(int index) = 0;
    virtual void removeDataByID(int id) = 0;
    virtual void removeDataByName(const QString& name, bool compSens = false) = 0;
    virtual QString checkName(const QString& name) = 0;
    virtual int getIndexByID(int dataId) = 0;
    virtual int getIDByIndex(int dataIndex) = 0;
    virtual void sortObject(ObjectSortType type) = 0;
    virtual void sortObject(std::function<bool(FCAbstractDataObject*, FCAbstractDataObject*)> function) = 0;
    virtual bool isUsedDataObject(const QList<int>& data) = 0;
};

inline FCAbstractDataManagerPrivate::~FCAbstractDataManagerPrivate() = default;

} // namespace FC

#endif // FCABSTRACTDATAMANAGERPRIVATE_H
