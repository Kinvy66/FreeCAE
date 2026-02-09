/**
 * @file FCGeoModelSphere.cpp
 * @brief 球体几何命令实现（接口层）
 */
#include "FCAbsGeoModelSphere.h"
#include <FCModelInterface/FCAbstractModel.h>

namespace FC {

FCGeoEnum::FCGeometryComType FCGeoModelSphere::getGeometryCommandType()
{
    return FCGeoEnum::FGTSphere;
}

bool FCGeoModelSphere::update()
{
    return true;
}

void FCGeoModelSphere::setLocation(const double* p1)
{
    if (p1) FCAbstractModel::CopyArray(const_cast<double*>(p1), _location, 3);
}

void FCGeoModelSphere::getLocation(double* p1) const
{
    if (p1) FCAbstractModel::CopyArray(const_cast<double*>(_location), p1, 3);
}

void FCGeoModelSphere::setRadius(double r) { _radius = r; }
double FCGeoModelSphere::getRadius() const { return _radius; }

} // namespace FC
