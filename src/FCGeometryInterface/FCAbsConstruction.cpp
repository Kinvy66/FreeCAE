/**
 * @file FCGeoConstruction.cpp
 * @brief 构造线命令实现（接口层�?
 */
#include "FCAbsConstruction.h"
#include <FCModelInterface/FCAbstractModel.h>

namespace FC {

FCGeoConstruction::FCGeoConstruction(FCGeoEnum::FITKConstructionType type)
    : m_type(type)
{
}

FCGeoEnum::FITKGeometryComType FCGeoConstruction::getGeometryCommandType()
{
    return FCGeoEnum::FGTConstruction;
}

bool FCGeoConstruction::update()
{
    return true;
}

void FCGeoConstruction::getPositions(double* point1, double* point2) const
{
    if (point1) FCAbstractModel::CopyArray(const_cast<double*>(m_point1), point1, 3);
    if (point2) FCAbstractModel::CopyArray(const_cast<double*>(m_point2), point2, 3);
}

void FCGeoConstruction::setPositions(const double* point1, const double* point2)
{
    if (point1) FCAbstractModel::CopyArray(const_cast<double*>(point1), m_point1, 3);
    if (point2) FCAbstractModel::CopyArray(const_cast<double*>(point2), m_point2, 3);
}

void FCGeoConstruction::setPosition1(double x, double y, double z)
{
    m_point1[0] = x; m_point1[1] = y; m_point1[2] = z;
}

void FCGeoConstruction::setPosition1(const double* point)
{
    if (point) FCAbstractModel::CopyArray(const_cast<double*>(point), m_point1, 3);
}

void FCGeoConstruction::setPosition2(double x, double y, double z)
{
    m_point2[0] = x; m_point2[1] = y; m_point2[2] = z;
}

void FCGeoConstruction::setPosition2(const double* point)
{
    if (point) FCAbstractModel::CopyArray(const_cast<double*>(point), m_point2, 3);
}

} // namespace FC
