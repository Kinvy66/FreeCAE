/**
 * @file FCGeoModelNearestPointLine.cpp
 * @brief 最近点线命令实现（接口层）
 */
#include "FCGeoModelNearestPointLine.h"
#include <FCModelInterface/FCAbstractModel.h>

namespace FC {

FCGeoEnum::FITKGeometryComType FCGeoModelNearestPointLine::getGeometryCommandType()
{
    return FCGeoEnum::FGTNearestPointLine;
}

bool FCGeoModelNearestPointLine::update()
{
    return true;
}

void FCGeoModelNearestPointLine::setPoint(const double* point)
{
    if (point) FCAbstractModel::CopyArray(const_cast<double*>(point), m_point, 3);
}

void FCGeoModelNearestPointLine::getPoint(double* point) const
{
    if (point) FCAbstractModel::CopyArray(const_cast<double*>(m_point), point, 3);
}

void FCGeoModelNearestPointLine::setNearestPoint(const double* nearestPoint)
{
    if (nearestPoint) FCAbstractModel::CopyArray(const_cast<double*>(nearestPoint), m_nearestPoint, 3);
}

void FCGeoModelNearestPointLine::getNearestPoint(double* nearestPoint) const
{
    if (nearestPoint) FCAbstractModel::CopyArray(const_cast<double*>(m_nearestPoint), nearestPoint, 3);
}

} // namespace FC
