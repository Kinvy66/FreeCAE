/**
 * @file FCVec3Alg.h
 * @brief 三维向量/点运算（替代 FITKVec3DAlg，不依赖 FITKCore）
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#ifndef FCVEC3ALG_H
#define FCVEC3ALG_H

#include "FCPoint.h"
#include <cmath>

namespace FC {

/** 两点距离 */
inline double Distance(const FCPoint& a, const FCPoint& b)
{
    double dx = a.x() - b.x();
    double dy = a.y() - b.y();
    double dz = a.z() - b.z();
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

/** 向量差 b - a，结果写入点形式 */
inline FCPoint Subtract(const FCPoint& b, const FCPoint& a)
{
    return FCPoint(b.x() - a.x(), b.y() - a.y(), b.z() - a.z());
}

/** 叉积，返回新点 */
inline FCPoint CrossProduct(const FCPoint& a, const FCPoint& b)
{
    return FCPoint(
        a.y() * b.z() - a.z() * b.y(),
        a.z() * b.x() - a.x() * b.z(),
        a.x() * b.y() - a.y() * b.x());
}

/** 点绕轴旋转：点 p 绕过原点、方向为 axis 的轴旋转 angle 弧度，返回新点 */
inline FCPoint RotateAroundAxis(const FCPoint& p, const FCPoint& axis, double angleRad)
{
    double ax = axis.x(), ay = axis.y(), az = axis.z();
    double n2 = ax * ax + ay * ay + az * az;
    if (n2 < 1e-30)
        return p;
    double n = std::sqrt(n2);
    ax /= n; ay /= n; az /= n;
    double c = std::cos(angleRad), s = std::sin(angleRad);
    double ux = p.x(), uy = p.y(), uz = p.z();
    double t = (1 - c) * (ax * ux + ay * uy + az * uz);
    return FCPoint(
        c * ux + t * ax + s * (ay * uz - az * uy),
        c * uy + t * ay + s * (az * ux - ax * uz),
        c * uz + t * az + s * (ax * uy - ay * ux));
}

} // namespace FC

#endif // FCVEC3ALG_H
