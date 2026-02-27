/**
 * @file FCAbstractDataManagerHelper.h
 * @brief 数据管理器辅助实现（替代 FITKAbstractDataManagerHelper）
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#ifndef FCABSTRACTDATAMANAGERHELPER_H
#define FCABSTRACTDATAMANAGERHELPER_H

#include "FCDataAPI.h"
#include "FCType.h"
#include "FCAbstractDataManagerPrivate.h"
#include "FCAbstractDataObject.h"
#include "FCAbstractNamedDataObject.h"
#include <QList>
#include <QObject>
#include <QString>
#include <functional>

namespace FC {

class FCDATA_API FCAbstractDataManagerHelper : public QObject
{
    Q_OBJECT
public:
    explicit FCAbstractDataManagerHelper(QList<FCAbstractDataObject*>& list);
    ~FCAbstractDataManagerHelper() override = default;

    void appendData(FCAbstractDataObject* obj);
    void appendData(const QList<FCAbstractDataObject*>& objs);
    int getDataCount() const;
    bool isContains(FCAbstractDataObject* obj) const;
    void clear();
    void insertDataObj(int index, FCAbstractDataObject* item);
    FCAbstractDataObject* getDataByIndex(int index);
    FCAbstractDataObject* getDataByID(FCID id);
    FCAbstractDataObject* getDataByName(const QString& name, bool compSens = false);
    QList<FCAbstractDataObject*> getDataListByName(const QString& name, bool compSens = false);
    int getDataIndex(FCAbstractDataObject* obj) const;
    void removeDataObj(FCAbstractDataObject* obj);
    void removeDataObjWithoutRelease(FCAbstractDataObject* obj);
    void removeDataByIndex(int index);
    void removeDataByID(FCID id);
    void removeDataByName(const QString& name, bool compSens = false);
    void setParentDataID(FCID id);
    FCID getParentDataID() const;
    QString checkName(const QString& name);
    int getIndexByID(FCID dataId) const;
    FCID getIDByIndex(int dataIndex) const;
    void sortObject(ObjectSortType type);
    void sortObject(std::function<bool(FCAbstractDataObject*, FCAbstractDataObject*)> function);
    bool isUsedDataObject(const QList<FCID>& data) const;

public slots:
    void dataObjectDestoried(FCAbstractDataObject* obj);

private:
    void connectHelper(FCAbstractDataObject* obj, bool con = true);

    QList<FCAbstractDataObject*>& mDataList;
    FCID mParentDataID{ FCID_INVALID };
};

} // namespace FC

#endif // FCABSTRACTDATAMANAGERHELPER_H
