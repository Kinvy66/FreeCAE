/**
 * @file FCGeoCommandProp.cpp
 * @brief 几何命令属性实现（移植自 FITKInterfaceGeometry）
 */
#include "FCGeoCommandProp.h"

namespace FC {

unsigned int FCGeoCommandProp::_instanceCount = 0;
uint64_t FCGeoCommandProp::_default = 0;

FCGeoCommandProp::FCGeoCommandProp()
    : m_number(_default)
{
    m_color = QColor::fromHsv((_instanceCount * 137) % 360, 200, 230);
    ++_instanceCount;
}

FCGeoCommandProp::~FCGeoCommandProp()
{}

void FCGeoCommandProp::setColor(const QColor& color)
{
    m_color = color;
}

QColor FCGeoCommandProp::getColor() const
{
    return m_color;
}

uint64_t FCGeoCommandProp::getNumber() const
{
    return m_number;
}

void FCGeoCommandProp::setNumber(uint64_t number)
{
    m_number = number;
}

void FCGeoCommandProp::setDefault(uint64_t defaultValue)
{
    _default = defaultValue;
}

uint64_t FCGeoCommandProp::getDefault()
{
    return _default;
}

bool FCGeoCommandProp::bitwiseAnd(uint64_t other) const
{
    return (m_number & other) != 0;
}

void FCGeoCommandProp::bitwiseOr(uint64_t other)
{
    m_number |= other;
}

} // namespace FC
