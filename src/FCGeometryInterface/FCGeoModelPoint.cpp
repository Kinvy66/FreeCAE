/**
 * @file FCGeoModelPoint.cpp
 * @brief 点几何命令实现（接口层）
 */
#include "FCGeoModelPoint.h"

namespace FC {

FCGeoModelPoint::FCGeoModelPoint(double x, double y, double z)
    : m_x(x), m_y(y), m_z(z)
{
}

FCGeoEnum::FITKGeometryComType FCGeoModelPoint::getGeometryCommandType()
{
    return FCGeoEnum::FGTPoint;
}

bool FCGeoModelPoint::update()
{
    return true;
}

void FCGeoModelPoint::setCoord(double x, double y, double z)
{
    m_x = x;
    m_y = y;
    m_z = z;
}

FCGeoEnum::FITKGeometryComType FCGeoModelCurveEnd::getGeometryCommandType()
{
    return FCGeoEnum::FGTCurveEnd;
}

bool FCGeoModelCurveEnd::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelCurveEndDistance::getGeometryCommandType()
{
    return FCGeoEnum::FGTCurveEndDistance;
}

bool FCGeoModelCurveEndDistance::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelCurveRadio::getGeometryCommandType()
{
    return FCGeoEnum::FGTCurveRadio;
}

bool FCGeoModelCurveRadio::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelInteractionPoint::getGeometryCommandType()
{
    return FCGeoEnum::FGTInteractionPoint;
}

bool FCGeoModelInteractionPoint::update() { return true; }

} // namespace FC
