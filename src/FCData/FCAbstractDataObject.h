/**
 * @file FCAbstractDataObject.h
 * @brief 数据对象基类（替代 FITKAbstractDataObject，不依赖 FITKCore）
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#ifndef FCABSTRACTDATAOBJECT_H
#define FCABSTRACTDATAOBJECT_H

#include "FCDataAPI.h"
#include "FCType.h"
#include "FCAbstractObject.hpp"
#include <QObject>
#include <QHash>
#include <QVariant>

namespace FC {

class FCDataRepo;

class FCDATA_API FCAbstractDataObject : public QObject, public FCAbstractObject
{
    Q_OBJECT
    friend class FCDataRepo;
public:
    enum UserDataRole {
        UserRole = 1,
        UserRoleEnd = 1000000
    };

    explicit FCAbstractDataObject(int parentDataID = -1);
    virtual ~FCAbstractDataObject() override = 0;

    /**
     * @brief 获取数据对象的ID
     * @return int  数据ID
     */
    int getDataObjectID() const;

    /**
     * @brief 设置父数据对象id
     * @param[i]  id 父数据对象id
     */
    virtual void setParentDataID(int id);

    /**
     * @brief 获取父数据对象id
     * @return int 父数据对象id
     */
    int getParentDataID() const;

    /**
     * @brief 序列化对象
     * @param[i] label 标记位，标记序列化类型
     * @return QString 序列化后的字符串
     */
    virtual QString serialize(int label = -1);
    
    /**
     * @brief 反序列化对象
     * @param[i] text 序列化后的字符串
     * @param[i] label 标记位，标记序列化类型
     * @return bool 是否反序列化成功
     */
    virtual bool deserialize(const QString& text, int label = -1);

    /**
     * @brief 获取用户数据
     * @param[i] userData 用户数据
     * @return QVariant 用户数据
     */
    QVariant getUserData(int userData) const;

    /**
     * @brief 设置用户数据
     * @param[i] ud 用户数据
     * @param[i] va 用户数据
     */
    virtual void setUserData(int ud, const QVariant& va);

    /**
     * @brief 获取用户数据，并进行强制类型转换
     * @param[i] ud 用户数据
     * @return T 用户数据
     */
    template<typename T>
    T getUserDataT(int ud) const { return getUserData(ud).value<T>(); }

    /**
     * @brief 获取用户数据键列表
     * @return QList<int> 用户数据键列表
     */
    QList<int> getUserDataKeys() const;

    /**
     * @brief 复制数据对象
     * @param[i] obj 被复制的对象
     * @return bool 是否复制成功
     */
    virtual bool copy(FCAbstractDataObject* obj);

    /**
     * @brief 获取父数据对象
     * @return FCAbstractDataObject* 父数据对象
     */
    FCAbstractDataObject* getParentObject();

    /**
     * @brief 获取父数据对象，并进行强制类型转换
     * @param[i] T 父数据对象类型
     * @return T* 父数据对象
     */
    template<typename T>
    T* getParentObjectT() {
        FCAbstractDataObject* p = getParentObject();
        while (p) {
            T* t = dynamic_cast<T*>(p);
            if (t) return t;
            p = p->getParentObject();
        }
        return nullptr;
    }

    /**
     * @brief 判断数据对象是否被使用
     * @param[i] dataIDs 数据对象ID列表
     * @return bool 是否被使用
     */
    virtual bool isUsedDataObject(const QList<int>& dataIDs);

    /**
     * @brief 获取数据对象名称
     * @return QString 数据对象名称
     */
    QString objectName() = delete;

    /**
     * @brief 设置数据对象名称
     * @param[i] name 数据对象名称
     */
    void setObjectName(const QString&) = delete;

signals:
    /**
     * @brief 数据对象销毁信号
     * @param[i] obj 销毁的数据对象
     */
    void dataObjectDestoried(FCAbstractDataObject* obj);

protected:
    /**
     * @brief 设置数据对象ID
     * @param[i] id 数据对象ID
     */
    virtual void setAbsDataID(int id);

private:
    FCID mAbsDataID{ 0 };
    FCID mParentDataID{ -1 };
    QHash<FCID, QVariant> mUserData;
};

} // namespace FC

#endif // FCABSTRACTDATAOBJECT_H
