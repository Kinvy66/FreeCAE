/**
 * @file FCGeoModelBox.cpp
 * @brief 长方体几何命令实现（接口层，具体几何由 OCC 等后端实现）
 */
#include "FCGeoModelBox.h"
#include <FCModelInterface/FCAbstractModel.h>

namespace FC {

FCGeoEnum::FITKGeometryComType FCGeoModelBox::getGeometryCommandType()
{
    return FCGeoEnum::FGTBox;
}

bool FCGeoModelBox::update()
{
    return true;
}

void FCGeoModelBox::setPoint1(const double* p1)
{
    if (p1) FCAbstractModel::CopyArray(const_cast<double*>(p1), _point1, 3);
}

void FCGeoModelBox::setLength(const double* length)
{
    if (length) FCAbstractModel::CopyArray(const_cast<double*>(length), _lengthParam, 3);
}

void FCGeoModelBox::getPoint1(double* p1) const
{
    if (p1) FCAbstractModel::CopyArray(const_cast<double*>(_point1), p1, 3);
}

void FCGeoModelBox::getLength(double* length) const
{
    if (length) FCAbstractModel::CopyArray(const_cast<double*>(_lengthParam), length, 3);
}

} // namespace FC
