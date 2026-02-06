/**
 * @file FCGeoModelExtractCenter.cpp
 * @brief 抽中面/圆心命令实现（接口层）
 */
#include "FCAbsGeoModelExtractCenter.h"
#include <FCModelInterface/FCAbstractModel.h>

namespace FC {

FCGeoEnum::FITKGeometryComType FCGeoModelExtractCenter::getGeometryCommandType()
{
    return FCGeoEnum::FGTExtractCenter;
}

bool FCGeoModelExtractCenter::update()
{
    return true;
}

void FCGeoModelExtractCenter::setCenter(const double* center)
{
    if (center) FCAbstractModel::CopyArray(const_cast<double*>(center), m_center, 3);
}

void FCGeoModelExtractCenter::getCenter(double* center) const
{
    if (center) FCAbstractModel::CopyArray(const_cast<double*>(m_center), center, 3);
}

} // namespace FC
