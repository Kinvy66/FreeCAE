/**
 * @file FCGeoOpType.h
 * @brief 几何操作类型（函数式 DAG 节点类型）
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#ifndef FCGEOOPTYPE_H
#define FCGEOOPTYPE_H

#include "FCGeometryEntityAPI.h"
#include <QObject>

namespace FC {

/**
 * @brief 几何操作类型
 * 对应《函数式几何模块设计指导文档》中的 GeoOpType
 */
enum class FCGeoOpType
{
    None = 0,
    Block,
    Cylinder,
    Sphere,
    Union,
    Difference,
    Intersection,
    Fillet,
    Chamfer,
    Import,
    Extrude,
    Revolve,
    Sweep
};

inline uint qHash(FCGeoOpType type, uint seed = 0)
{
    return seed ^ static_cast<uint>(static_cast<int>(type));
}

} // namespace FC

Q_DECLARE_METATYPE(FC::FCGeoOpType)

#endif // FCGEOOPTYPE_H
