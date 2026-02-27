/**
 * @file FCAbstractDataManagerPrivate.h
 * @brief 数据管理器抽象接口（替代 FITKAbstractDataManagerPrivate）
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#ifndef FCABSTRACTDATAMANAGERPRIVATE_H
#define FCABSTRACTDATAMANAGERPRIVATE_H

#include "FCDataAPI.h"
#include "FCType.h"
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

     /**
     * @brief 追加对象
     * @param[i] item          追加的数据对象
     */
    virtual void appendDataObj(FCAbstractDataObject* item) = 0;

    /**
    * @brief 批量追加对象
    * @param[i] items         追加的数据对象
    */
    virtual void appendDataObj(const QList<FCAbstractDataObject*>& items) = 0;

    /**
     * @brief 获取管理的数据数量
     * @return int 数据数量
     */
    virtual int getDataCount() const = 0;

    /**
     * @brief 是否包含数据对象
     * @param[i] obj        待判定对象指针
     * @return true   包含该对象
     * @return false  不包含该对象
     */
    virtual bool isContains(FCAbstractDataObject* obj) const = 0;
    
    /**
     * @brief 清空管理器
     */ 
    virtual void clear() = 0;

    /**
     * @brief 插入数据对象
     * @param[i] index         插入的索引
     * @param[i] item          插入的数据对象
     */
    virtual void insertDataObj(int index, FCAbstractDataObject* item) = 0;

    /**
     * @brief 获取数据对象的索引
     * @param[i] obj        待获取索引的数据对象
     * @return int 数据对象的索引
     */
    virtual int getDataIndex(FCAbstractDataObject* obj) const = 0;

    /**
     * @brief 获取数据对象的名称
     * @param[i] name        数据对象的名称
     * @param[i] compSens    是否区分大小写
     * @return FCAbstractDataObject* 数据对象
     */
    virtual FCAbstractDataObject* getDataObjectByName(const QString& name, bool compSens = false) = 0;

    /**
     * @brief 获取数据对象的名称列表
     * @param[i] name        数据对象的名称
     * @param[i] compSens    是否区分大小写
     * @return QList<FCAbstractDataObject*> 数据对象列表
     */
    virtual QList<FCAbstractDataObject*> getDataObjectsByName(const QString& name, bool compSens = false) = 0;

    /**
     * @brief 删除数据对象
     * @param[i] obj        待删除的数据对象
     */
    virtual void removeDataObj(FCAbstractDataObject* obj) = 0;

    /**
     * @brief 删除数据对象，不释放对象
     * @param[i] obj        待删除的数据对象
     */
    virtual void removeDataObjWithoutRelease(FCAbstractDataObject* obj) = 0;

    /**
     * @brief 删除数据对象
     * @param[i] index         删除的索引
     */
    virtual void removeDataByIndex(int index) = 0;

    /**
     * @brief 删除数据对象
     * @param[i] id        删除的数据对象ID
     */
    virtual void removeDataByID(int id) = 0;

    /**
     * @brief 删除数据对象
     * @param[i] name        删除的数据对象名称
     * @param[i] compSens    是否区分大小写
     */
    virtual void removeDataByName(const QString& name, bool compSens = false) = 0;

    /**
     * @brief 检查数据对象名称
     * @param[i] name        检查的数据对象名称
     * @return QString 检查后的数据对象名称
     */
    virtual QString checkName(const QString& name) = 0;

    /**
     * @brief 获取数据对象的索引
     * @param[i] dataId        获取的数据对象ID
     * @return int 数据对象的索引
     */
    virtual int getIndexByID(int dataId) = 0;

    /**
     * @brief 获取数据对象的ID
     * @param[i] dataIndex        获取的数据对象索引
     * @return int 数据对象的ID
     */
    virtual int getIDByIndex(int dataIndex) = 0;

    /**
     * @brief 排序数据对象
     * @param[i] type        排序类型
     */
    virtual void sortObject(ObjectSortType type) = 0;

    /**
     * @brief 排序数据对象
     * @param[i] function        排序函数
     */
    virtual void sortObject(std::function<bool(FCAbstractDataObject*, FCAbstractDataObject*)> function) = 0;

    /**
     * @brief 判断数据对象是否被使用
     * @param[i] data        数据对象ID列表
     * @return bool 是否被使用
     */
    virtual bool isUsedDataObject(const QList<FCID>& data) = 0;
};

inline FCAbstractDataManagerPrivate::~FCAbstractDataManagerPrivate() = default;

} // namespace FC

#endif // FCABSTRACTDATAMANAGERPRIVATE_H
