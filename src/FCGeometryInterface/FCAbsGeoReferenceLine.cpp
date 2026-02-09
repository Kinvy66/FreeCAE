/**
 * @file FCGeoReferenceLine.cpp
  * @brief 参考线命令实现（接口层）
 */
#include "FCAbsGeoReferenceLine.h"

namespace FC {

FCGeoEnum::FITKDatumType FCGeoReferenceLine::getDatumType()
{
    return FCGeoEnum::FDTLine;
}

FCGeoEnum::FITKDatumType FCGeoReferenceSegment::getDatumType()
{
    return FCGeoEnum::FDTSegment;
}

void FCGeoReferenceSegment::setStartPoint(double x, double y, double z)
{
    _pos[0] = x; _pos[1] = y; _pos[2] = z;
}

void FCGeoReferenceSegment::setStartPoint(std::array<double, 3> xyz)
{
    _pos[0] = xyz[0]; _pos[1] = xyz[1]; _pos[2] = xyz[2];
}

void FCGeoReferenceSegment::setEndPoint(double x, double y, double z)
{
    _pos2[0] = x; _pos2[1] = y; _pos2[2] = z;
}

void FCGeoReferenceSegment::setEndPoint(std::array<double, 3> xyz)
{
    _pos2[0] = xyz[0]; _pos2[1] = xyz[1]; _pos2[2] = xyz[2];
}

FCGeoEnum::FITKDatumType FCGeoReferenceLineByIntersectPlanes::getDatumType()
{
    return FCGeoEnum::FDTLineByIntersectPlanes;
}

FCGeoEnum::FITKDatumType FCGeoReferenceLineByRotateLine::getDatumType()
{
    return FCGeoEnum::FDTLineByRotateLine;
}

void FCGeoReferenceLineByRotateLine::setLine(const QVector<double>& start, const QVector<double>& end)
{
    if (start.size() >= 3) m_lineStart = { start[0], start[1], start[2] };
    if (end.size() >= 3) m_lineEnd = { end[0], end[1], end[2] };
}

void FCGeoReferenceLineByRotateLine::setAxis(const QVector<double>& start, const QVector<double>& end)
{
    if (start.size() >= 3) m_axisStart = { start[0], start[1], start[2] };
    if (end.size() >= 3) m_axisEnd = { end[0], end[1], end[2] };
}

} // namespace FC
