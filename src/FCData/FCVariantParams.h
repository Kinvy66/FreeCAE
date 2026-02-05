/**
 * @file FCVariantParams.h
 * @brief 参数管理（QHash<QString,QVariant>，替代 FITKVarientParams）
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#ifndef FCVARIANT_PARAMS_H
#define FCVARIANT_PARAMS_H

#include "FCDataAPI.h"
#include <QHash>
#include <QString>
#include <QVariant>

namespace FC {

class FCDATA_API FCVariantParams
{
public:
    explicit FCVariantParams() = default;
    virtual ~FCVariantParams() = default;

    void setValue(const QString& k, const QVariant& v);
    QVariant getValue(const QString& k) const;

    template<typename T>
    T getValueT(const QString& k) const
    {
        return getValue(k).value<T>();
    }

    void removeValue(const QString& k);
    void clear();

private:
    QHash<QString, QVariant> _settings;
};

} // namespace FC

#endif // FCVARIANT_PARAMS_H
