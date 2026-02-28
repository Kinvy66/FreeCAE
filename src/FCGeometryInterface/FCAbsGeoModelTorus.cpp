/**
 * @file FCAbsGeoModelTorus.cpp
 * @brief 圆环几何命令实现（接口层）
 * @date 2026-02-28
 * @version V0.0.1
 * @details
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#include "FCAbsGeoModelTorus.h"
#include <FCModelInterface/FCAbstractModel.h>

namespace FC
{

FCGeoEnum::FCGeometryComType FCGeoModelTorus::getGeometryCommandType()
{
    return FCGeoEnum::FGTTorus;
}

bool FCGeoModelTorus::update()
{
    return true;
}

void FCGeoModelTorus::setLocation(const double* p)
{
    if (p) FCAbstractModel::CopyArray(const_cast<double*>(p), mLocation, 3);
}

void FCGeoModelTorus::getLocation(double* p) const
{
    if (p) FCAbstractModel::CopyArray(const_cast<double*>(mLocation), p, 3);
}

void FCGeoModelTorus::setAxis(const double* axis)
{
    if (axis) FCAbstractModel::CopyArray(const_cast<double*>(axis), mAxis, 3);
}

void FCGeoModelTorus::getAxis(double* axis) const
{
    if (axis) FCAbstractModel::CopyArray(const_cast<double*>(mAxis), axis, 3);
}

void FCGeoModelTorus::setMajorRadius(double r) { mMajorRadius = r; }
double FCGeoModelTorus::getMajorRadius() const { return mMajorRadius; }
void FCGeoModelTorus::setMinorRadius(double r) { mMinorRadius = r; }
double FCGeoModelTorus::getMinorRadius() const { return mMinorRadius; }
void FCGeoModelTorus::setAngle(double angle) { mAngle = angle; }
double FCGeoModelTorus::getAngle() const { return mAngle; }

} // namespace FC
