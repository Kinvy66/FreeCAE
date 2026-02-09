/**
 * @file FCGeoModelPoint.cpp
 * @brief 点几何命令实现（接口层）
 */
#include "FCAbsGeoModelPoint.h"

namespace FC {

FCGeoModelPoint::FCGeoModelPoint(double x, double y, double z)
    : m_x(x), m_y(y), m_z(z)
{
}

FCGeoEnum::FCGeometryComType FCGeoModelPoint::getGeometryCommandType()
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

FCGeoEnum::FCGeometryComType FCGeoModelCurveEnd::getGeometryCommandType()
{
    return FCGeoEnum::FGTCurveEnd;
}

bool FCGeoModelCurveEnd::update() { return true; }

FCGeoEnum::FCGeometryComType FCGeoModelCurveEndDistance::getGeometryCommandType()
{
    return FCGeoEnum::FGTCurveEndDistance;
}

bool FCGeoModelCurveEndDistance::update() { return true; }

FCGeoEnum::FCGeometryComType FCGeoModelCurveRadio::getGeometryCommandType()
{
    return FCGeoEnum::FGTCurveRadio;
}

bool FCGeoModelCurveRadio::update() { return true; }

FCGeoEnum::FCGeometryComType FCGeoModelInteractionPoint::getGeometryCommandType()
{
    return FCGeoEnum::FGTInteractionPoint;
}

bool FCGeoModelInteractionPoint::update() { return true; }

FCGeoEnum::FCGeometryComType FCGeoModelPointProjectionOnCurve::getGeometryCommandType()
{
    return FCGeoEnum::FGTPointProjectionOnCurve;
}
bool FCGeoModelPointProjectionOnCurve::update() { return true; }

FCGeoEnum::FCGeometryComType FCGeoModelTwoCurveIntersectPoint::getGeometryCommandType()
{
    return FCGeoEnum::FGTTwoCurveIntersectPoint;
}
bool FCGeoModelTwoCurveIntersectPoint::update() { return true; }

FCGeoEnum::FCGeometryComType FCGeoModelPointProjectionOnSurface::getGeometryCommandType()
{
    return FCGeoEnum::FGTPointProjectionOnSurface;
}
bool FCGeoModelPointProjectionOnSurface::update() { return true; }

FCGeoEnum::FCGeometryComType FCGeoModelCurveIntersectSurface::getGeometryCommandType()
{
    return FCGeoEnum::FGTCurveIntersectSurface;
}
bool FCGeoModelCurveIntersectSurface::update() { return true; }

} // namespace FC
