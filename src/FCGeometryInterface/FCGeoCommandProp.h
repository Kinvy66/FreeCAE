/**
 * @file FCGeoCommandProp.h
 * @brief 几何命令属性（移植自 FITKInterfaceGeometry）
 */
#ifndef FCGEOCOMMANDPROP_H
#define FCGEOCOMMANDPROP_H

#include "FCGeometryInterfaceAPI.h"
#include <QColor>

namespace FC {

class FCGEOMETRYINTERFACE_API FCGeoCommandProp
{
public:
    explicit FCGeoCommandProp();
    ~FCGeoCommandProp();

    QColor getColor() const;
    void setColor(const QColor& color);
    uint64_t getNumber() const;
    void setNumber(uint64_t number);
    static uint64_t getDefault();
    static void setDefault(uint64_t defaultValue);
    bool bitwiseAnd(uint64_t other) const;
    void bitwiseOr(uint64_t other);

private:
    QColor m_color;
    uint64_t m_number{ 0 };
    static unsigned int _instanceCount;
    static uint64_t _default;
};

} // namespace FC

#endif // FCGEOCOMMANDPROP_H
