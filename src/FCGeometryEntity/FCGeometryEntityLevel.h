/**
 * @file FCGeometryEntityLevel.h
 * @brief 几何实体层级枚举（类似 COMSOL Domain/Boundary/Edge/Point）
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#ifndef FCGEOMETRYENTITYLEVEL_H
#define FCGEOMETRYENTITYLEVEL_H

#include "FCGeometryEntityAPI.h"
#include <QString>

namespace FC {

/**
 * @brief 几何实体层级
 * 与 COMSOL 的 Geometric Entity Level 对应，供网格/物理/后处理统一选择
 */
enum class FCGeometryEntityLevel
{
    None = 0,       /**< 无/未指定 */
    Domain = 1,     /**< 体（3D）或面（2D）或线（1D）*/
    Boundary = 2,   /**< 面（3D）或边（2D）或点（1D）*/
    Edge = 3,       /**< 边（3D）或点（2D）*/
    Point = 4       /**< 点 */
};

} // namespace FC

#endif // FCGEOMETRYENTITYLEVEL_H
