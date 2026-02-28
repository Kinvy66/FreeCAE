/**
 * @file FCAbsGeoModelCone.cpp
 * @brief 圆锥(台)几何命令实现（接口层）
 * @date 2026-02-28
 * @version V0.0.1
 * @details
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#include "FCAbsGeoModelCone.h"
#include <FCModelInterface/FCAbstractModel.h>

namespace FC
{

FCGeoEnum::FCGeometryComType FCGeoModelCone::getGeometryCommandType()
{
    return FCGeoEnum::FGTCone;
}

bool FCGeoModelCone::update()
{
    return true;
}

void FCGeoModelCone::setLocation(const double* p)
{
    if (p) FCAbstractModel::CopyArray(const_cast<double*>(p), mLocation, 3);
}

void FCGeoModelCone::getLocation(double* p) const
{
    if (p) FCAbstractModel::CopyArray(const_cast<double*>(mLocation), p, 3);
}

void FCGeoModelCone::setAxis(const double* axis)
{
    if (axis) FCAbstractModel::CopyArray(const_cast<double*>(axis), mAxis, 3);
}

void FCGeoModelCone::getAxis(double* axis) const
{
    if (axis) FCAbstractModel::CopyArray(const_cast<double*>(mAxis), axis, 3);
}

void FCGeoModelCone::setHeight(double h) { mHeight = h; }
double FCGeoModelCone::getHeight() const { return mHeight; }
void FCGeoModelCone::setBottomRadius(double r) { mBottomRadius = r; }
double FCGeoModelCone::getBottomRadius() const { return mBottomRadius; }
void FCGeoModelCone::setTopRadius(double r) { mTopRadius = r; }
double FCGeoModelCone::getTopRadius() const { return mTopRadius; }

} // namespace FC
