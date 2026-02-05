/**
 * @file FCAbstractNamedDataObject.h
 * @brief 具名数据对象基类（替代 FITKAbstractNDataObject）
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#ifndef FCABSTRACTNAMEDDATAOBJECT_H
#define FCABSTRACTNAMEDDATAOBJECT_H

#include "FCAbstractDataObject.h"
#include <QString>

namespace FC {

class FCDATA_API FCAbstractNamedDataObject : public FCAbstractDataObject
{
public:
    explicit FCAbstractNamedDataObject() = default;
    explicit FCAbstractNamedDataObject(const QString& name);
    ~FCAbstractNamedDataObject() override = 0;

    virtual void setDataObjectName(const QString& name);
    virtual QString getDataObjectName() const;

    bool copy(FCAbstractDataObject* obj) override;

private:
    QString m_dataObjName;
};

} // namespace FC

#endif // FCABSTRACTNAMEDDATAOBJECT_H
