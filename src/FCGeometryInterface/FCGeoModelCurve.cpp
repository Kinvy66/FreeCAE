/**
 * @file FCGeoModelCurve.cpp
 * @brief 曲线几何命令实现（接口层）
 */
#include "FCGeoModelCurve.h"

namespace FC {

void FCGeoModelCurve::setOrigin(double x, double y, double z)
{
    m_origin = { x, y, z };
}

void FCGeoModelCurve::setYDirection(std::array<double, 3> xyz)
{
    m_yDirection = xyz;
    m_xDirection = {
        m_yDirection[1] * m_zDirection[2] - m_zDirection[1] * m_yDirection[2],
        m_zDirection[0] * m_yDirection[2] - m_yDirection[0] * m_zDirection[2],
        m_yDirection[0] * m_zDirection[1] - m_zDirection[0] * m_yDirection[1]
    };
}

void FCGeoModelCurve::setYDirection(double x, double y, double z)
{
    setYDirection({ x, y, z });
}

void FCGeoModelCurve::setZDirection(std::array<double, 3> xyz)
{
    m_zDirection = xyz;
    m_xDirection = {
        m_yDirection[1] * m_zDirection[2] - m_zDirection[1] * m_yDirection[2],
        m_zDirection[0] * m_yDirection[2] - m_yDirection[0] * m_zDirection[2],
        m_yDirection[0] * m_zDirection[1] - m_zDirection[0] * m_yDirection[1]
    };
}

void FCGeoModelCurve::setZDirection(double x, double y, double z)
{
    setZDirection({ x, y, z });
}

FCGeoEnum::FITKGeometryComType FCGeoModelLine::getGeometryCommandType()
{
    return FCGeoEnum::FGTLine;
}
bool FCGeoModelLine::update() { return true; }

void FCGeoModelLine::setDir(double x, double y, double z)
{
    m_dir = { x, y, z };
}

FCGeoEnum::FITKGeometryComType FCGeoModelSegment::getGeometryCommandType()
{
    return FCGeoEnum::FGTSegment;
}
bool FCGeoModelSegment::update() { return true; }

void FCGeoModelSegment::setStartPoint(double x, double y, double z)
{
    m_startPoint = { x, y, z };
}

void FCGeoModelSegment::setEndPoint(double x, double y, double z)
{
    m_endPoint = { x, y, z };
}

FCGeoEnum::FITKGeometryComType FCGeoModelWire::getGeometryCommandType()
{
    return FCGeoEnum::FGTWire;
}
bool FCGeoModelWire::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelCircle::getGeometryCommandType()
{
    return FCGeoEnum::FGTCircle;
}
bool FCGeoModelCircle::update() { return true; }

void FCGeoModelCircle::setCenter(double x, double y, double z)
{
    m_center = { x, y, z };
}

} // namespace FC
