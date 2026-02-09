/**
 * @file FCGeoModelCylinder.cpp
 * @brief 圆柱体几何命令实现（接口层）
 */
#include "FCAbsGeoModelCylinder.h"
#include "FCAbstractModel.h"

namespace FC {

FCGeoEnum::FCGeometryComType FCGeoModelCylinder::getGeometryCommandType()
{
    return FCGeoEnum::FGTCylinder;
}

bool FCGeoModelCylinder::update()
{
    return true;
}

void FCGeoModelCylinder::setLocation(const double* p1)
{
    if (p1) FCAbstractModel::CopyArray(const_cast<double*>(p1), _location, 3);
}

void FCGeoModelCylinder::getLocation(double* p1) const
{
    if (p1) FCAbstractModel::CopyArray(const_cast<double*>(_location), p1, 3);
}

void FCGeoModelCylinder::setDirection(const double* dir)
{
    if (dir) FCAbstractModel::CopyArray(const_cast<double*>(dir), _direction, 3);
}

void FCGeoModelCylinder::getDirection(double* dir) const
{
    if (dir) FCAbstractModel::CopyArray(const_cast<double*>(_direction), dir, 3);
}

void FCGeoModelCylinder::setRadius(double r) { _radius = r; }
double FCGeoModelCylinder::getRadius() const { return _radius; }
void FCGeoModelCylinder::setLength(double length) { _length = length; }
double FCGeoModelCylinder::getLength() const { return _length; }

} // namespace FC
