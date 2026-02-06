/**
 * @file FCGeoModelCurve.cpp
 * @brief 曲线几何命令实现（接口层）
 */
#include "FCAbsGeoModelCurve.h"

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

FCGeoEnum::FITKGeometryComType FCGeoModelThreePointsCircle::getGeometryCommandType()
{
    return FCGeoEnum::FGTThreePointsCircle;
}
bool FCGeoModelThreePointsCircle::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelCircleArc::getGeometryCommandType()
{
    return FCGeoEnum::FGTCircleArc;
}
bool FCGeoModelCircleArc::update() { return true; }
void FCGeoModelCircleArc::setStartPoint(double x, double y, double z)
{
    m_startPoint = { x, y, z };
}
void FCGeoModelCircleArc::setEndPoint(double x, double y, double z)
{
    m_endPoint = { x, y, z };
}

FCGeoEnum::FITKGeometryComType FCGeoModelEllipse::getGeometryCommandType()
{
    return FCGeoEnum::FGTEllipse;
}
bool FCGeoModelEllipse::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelThreePointsEllipse::getGeometryCommandType()
{
    return FCGeoEnum::FGTThreePointsEllipse;
}
bool FCGeoModelThreePointsEllipse::update() { return true; }
void FCGeoModelThreePointsEllipse::setMajorPoint(double x, double y, double z)
{
    m_majorPoint = { x, y, z };
}
void FCGeoModelThreePointsEllipse::setMinorPoint(double x, double y, double z)
{
    m_minorPoint = { x, y, z };
}

FCGeoEnum::FITKGeometryComType FCGeoModelEllipseArc::getGeometryCommandType()
{
    return FCGeoEnum::FGTEllipseArc;
}
bool FCGeoModelEllipseArc::update() { return true; }
void FCGeoModelEllipseArc::setStartPoint(double x, double y, double z)
{
    m_startPoint = { x, y, z };
}
void FCGeoModelEllipseArc::setEndPoint(double x, double y, double z)
{
    m_endPoint = { x, y, z };
}

FCGeoEnum::FITKGeometryComType FCGeoModelHyperbola::getGeometryCommandType()
{
    return FCGeoEnum::FGTHyperbola;
}
bool FCGeoModelHyperbola::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelThreePointsHyperbola::getGeometryCommandType()
{
    return FCGeoEnum::FGTThreePointsHyperbola;
}
bool FCGeoModelThreePointsHyperbola::update() { return true; }
void FCGeoModelThreePointsHyperbola::setMajorPoint(double x, double y, double z)
{
    m_majorPoint = { x, y, z };
}
void FCGeoModelThreePointsHyperbola::setMinorPoint(double x, double y, double z)
{
    m_minorPoint = { x, y, z };
}

FCGeoEnum::FITKGeometryComType FCGeoModelHyperbolaArc::getGeometryCommandType()
{
    return FCGeoEnum::FGTHyperbolaArc;
}
bool FCGeoModelHyperbolaArc::update() { return true; }
void FCGeoModelHyperbolaArc::setStartPoint(double x, double y, double z)
{
    m_startPoint = { x, y, z };
}
void FCGeoModelHyperbolaArc::setEndPoint(double x, double y, double z)
{
    m_endPoint = { x, y, z };
}

FCGeoEnum::FITKGeometryComType FCGeoModelParabola::getGeometryCommandType()
{
    return FCGeoEnum::FGTParabola;
}
bool FCGeoModelParabola::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelThreePointsParabola::getGeometryCommandType()
{
    return FCGeoEnum::FGTThreePointsParabola;
}
bool FCGeoModelThreePointsParabola::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelParabolaArc::getGeometryCommandType()
{
    return FCGeoEnum::FGTParabolaArc;
}
bool FCGeoModelParabolaArc::update() { return true; }
void FCGeoModelParabolaArc::setStartPoint(double x, double y, double z)
{
    m_startPoint = { x, y, z };
}
void FCGeoModelParabolaArc::setEndPoint(double x, double y, double z)
{
    m_endPoint = { x, y, z };
}

FCGeoEnum::FITKGeometryComType FCGeoModelBezierByControlPoints::getGeometryCommandType()
{
    return FCGeoEnum::FGTBezierByControlPoints;
}
bool FCGeoModelBezierByControlPoints::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelBsplineByThroughPoints::getGeometryCommandType()
{
    return FCGeoEnum::FGTBSplineByThroughPoints;
}
bool FCGeoModelBsplineByThroughPoints::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelTrimmedCurve::getGeometryCommandType()
{
    return FCGeoEnum::FGTTrimmed;
}
bool FCGeoModelTrimmedCurve::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelOffsetCurve::getGeometryCommandType()
{
    return FCGeoEnum::FGTOffsetCurve;
}
bool FCGeoModelOffsetCurve::update() { return true; }
void FCGeoModelOffsetCurve::setDir(double x, double y, double z)
{
    m_dir = { x, y, z };
}

FCGeoEnum::FITKGeometryComType FCGeoModelCurveProjectionOnSurface::getGeometryCommandType()
{
    return FCGeoEnum::FGTCurveProjectionOnSurface;
}
bool FCGeoModelCurveProjectionOnSurface::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelSurfaceIntersectionSurface::getGeometryCommandType()
{
    return FCGeoEnum::FGTSurfaceIntersectSurface;
}
bool FCGeoModelSurfaceIntersectionSurface::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelBridgeCurve::getGeometryCommandType()
{
    return FCGeoEnum::FGTBridgeCurve;
}
bool FCGeoModelBridgeCurve::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelSurfaceEdge::getGeometryCommandType()
{
    return FCGeoEnum::FGTCurveFromSurface;
}
bool FCGeoModelSurfaceEdge::update() { return true; }

} // namespace FC
