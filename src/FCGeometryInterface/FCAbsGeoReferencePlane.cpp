/**
 * @file FCGeoReferencePlane.cpp
  * @brief 参考面命令实现（接口层）
 */
#include "FCAbsGeoReferencePlane.h"

namespace FC {

void FCGeoReferencePlane::setPosition(double x, double y, double z)
{
    _pos[0] = x; _pos[1] = y; _pos[2] = z;
}

void FCGeoReferencePlane::setNormal(double x, double y, double z)
{
    _nor[0] = x; _nor[1] = y; _nor[2] = z;
}

void FCGeoReferencePlane::setUp(double x, double y, double z)
{
    _up[0] = x; _up[1] = y; _up[2] = z;
}

FCGeoEnum::FITKDatumType FCGeoReferenceOffsetPlane::getDatumType()
{
    return FCGeoEnum::FDTOffsetPlane;
}

FCGeoEnum::FITKDatumType FCGeoReferenceThreePointsPlane::getDatumType()
{
    return FCGeoEnum::FDTThreePointsPlane;
}

std::array<double, 3> FCGeoReferenceThreePointsPlane::point(int index) const
{
    if (index >= 0 && index < 3)
        return m_points[index];
    return {};
}

void FCGeoReferenceThreePointsPlane::setPoint(int index, std::array<double, 3> xyz)
{
    if (index >= 0 && index < 3)
        m_points[index] = xyz;
}

void FCGeoReferenceThreePointsPlane::setPoint(int index, double x, double y, double z)
{
    if (index >= 0 && index < 3)
        m_points[index] = { x, y, z };
}

FCGeoEnum::FITKDatumType FCGeoReferenceEquationPlane::getDatumType()
{
    return FCGeoEnum::FDTEquationPlane;
}

FCGeoEnum::FITKDatumType FCGeoReferencePointAndDirectionPlane::getDatumType()
{
    return FCGeoEnum::FDTPointAndDirectionPlane;
}

FCGeoEnum::FITKDatumType FCGeoReferenceRotateFromPlanePlane::getDatumType()
{
    return FCGeoEnum::FDTRotateFromPlanePlane;
}

void FCGeoReferenceRotateFromPlanePlane::setPlane(const QVector<double>& plane)
{
    if (plane.size() >= 9) {
        m_planePos = { plane[0], plane[1], plane[2] };
        m_planeNor = { plane[3], plane[4], plane[5] };
        m_planeUp = { plane[6], plane[7], plane[8] };
    }
}

void FCGeoReferenceRotateFromPlanePlane::setAxis(const QVector<double>& start, const QVector<double>& end)
{
    if (start.size() >= 3) m_axisStart = { start[0], start[1], start[2] };
    if (end.size() >= 3) m_axisEnd = { end[0], end[1], end[2] };
}

} // namespace FC
