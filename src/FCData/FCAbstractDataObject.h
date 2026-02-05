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

    int getDataObjectID() const;
    virtual void setParentDataID(int id);
    int getParentDataID() const;

    virtual QString serialize(int label = -1);
    virtual bool deserialize(const QString& text, int label = -1);

    QVariant getUserData(int userData) const;
    virtual void setUserData(int ud, const QVariant& va);

    template<typename T>
    T getUserDataT(int ud) const { return getUserData(ud).value<T>(); }

    QList<int> getUserDataKeys() const;

    virtual bool copy(FCAbstractDataObject* obj);

    FCAbstractDataObject* getParentObject();

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

    virtual bool isUsedDataObject(const QList<int>& dataIDs);

    QString objectName() = delete;
    void setObjectName(const QString&) = delete;

signals:
    void dataObjectDestoried(FCAbstractDataObject* obj);

protected:
    virtual void setAbsDataID(int id);

private:
    FCID _absDataID{ 0 };
    FCID _parentDataID{ -1 };
    QHash<FCID, QVariant> _userData;
};

} // namespace FC

#endif // FCABSTRACTDATAOBJECT_H
