/**
 * @file FCAbsGeoModelHelix.cpp
 * @brief 螺旋体（接口层）
 * @date 2026-02-28
 * @version V0.0.1
 * @details
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#include "FCAbsGeoModelHelix.h"
#include <FCModelInterface/FCAbstractModel.h>

namespace FC
{

FCGeoEnum::FCGeometryComType FCGeoModelHelix::getGeometryCommandType()
{
    return FCGeoEnum::FGTHelix;
}

bool FCGeoModelHelix::update()
{
    return true;
}

void FCGeoModelHelix::setLocation(const double* p)
{
    if (p) FCAbstractModel::CopyArray(const_cast<double*>(p), mLocation, 3);
}

void FCGeoModelHelix::getLocation(double* p) const
{
    if (p) FCAbstractModel::CopyArray(const_cast<double*>(mLocation), p, 3);
}

void FCGeoModelHelix::setNumberOfTurns(int n) { mNumberOfTurns = n; }
int FCGeoModelHelix::getNumberOfTurns() const { return mNumberOfTurns; }
void FCGeoModelHelix::setMajorRadius(double r) { mMajorRadius = r; }
double FCGeoModelHelix::getMajorRadius() const { return mMajorRadius; }
void FCGeoModelHelix::setMinorRadius(double r) { mMinorRadius = r; }
double FCGeoModelHelix::getMinorRadius() const { return mMinorRadius; }
void FCGeoModelHelix::setRadialPitch(double p) { mRadialPitch = p; }
double FCGeoModelHelix::getRadialPitch() const { return mRadialPitch; }
void FCGeoModelHelix::setAxialPitch(double p) { mAxialPitch = p; }
double FCGeoModelHelix::getAxialPitch() const { return mAxialPitch; }
void FCGeoModelHelix::setChirality(int c) { mChirality = c; }
int FCGeoModelHelix::getChirality() const { return mChirality; }

} // namespace FC
