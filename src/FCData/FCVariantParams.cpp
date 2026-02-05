/**
 * @file FCVariantParams.cpp
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#include "FCVariantParams.h"

namespace FC {

void FCVariantParams::setValue(const QString& k, const QVariant& v)
{
    _settings.insert(k, v);
}

QVariant FCVariantParams::getValue(const QString& k) const
{
    return _settings.value(k);
}

void FCVariantParams::removeValue(const QString& k)
{
    _settings.remove(k);
}

void FCVariantParams::clear()
{
    _settings.clear();
}

} // namespace FC
