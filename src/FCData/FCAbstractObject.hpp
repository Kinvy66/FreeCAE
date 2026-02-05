/**
 * @file FCAbstractObject.hpp
 * @brief 抽象对象基类（不依赖 FITKCore）
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#ifndef FCABSTRACTOBJECT_HPP
#define FCABSTRACTOBJECT_HPP

#include "FCDataAPI.h"
#include "FCType.h"
#include <QString>
#include <QVariant>

namespace FC {

class FCDATA_API FCAbstractObject
{
public:
    explicit FCAbstractObject() = default;
    virtual ~FCAbstractObject() = 0;

    virtual bool isFCClass(const QString& className) const { Q_UNUSED(className); return false; }
    virtual QString getClassName() const { return QString(); }

    template<class T>
    T* safeDownCast() { return dynamic_cast<T*>(this); }

    template<class T>
    static T* safeDownCast(FCAbstractObject* obj) { return obj ? dynamic_cast<T*>(obj) : nullptr; }

    QVariant toVariant() const { return QVariant::fromValue(const_cast<FCAbstractObject*>(this)); }

    template<class T>
    static T* fromVariant(QVariant va) { return va.value<T*>(); }

    bool isEnable() const { return _objEnable; }
    void setEnable(bool en = true) { _objEnable = en; }

private:
    bool _objEnable{ true };
};

inline FCAbstractObject::~FCAbstractObject() = default;

} // namespace FC

Q_DECLARE_METATYPE(FC::FCAbstractObject*)

#endif // FCABSTRACTOBJECT_HPP
