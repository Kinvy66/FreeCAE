/**
 * @file FCGeoReferenceLine.h
 * @brief 参考线基准（移植自 FITKInterfaceGeometry FITKAbsGeoReferenceLine�?
 */
#ifndef FCGEOREFERENCELINE_H
#define FCGEOREFERENCELINE_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoDatum.h"
#include <QVector>
#include <array>

namespace FC {

/**
 * @brief 参考线基类
 */
class FCGEOMETRYINTERFACE_API FCGeoReferenceLine : public FCAbsGeoDatumLine
{
    FC_CLASS(FC, FCGeoReferenceLine);
public:
    FCGeoReferenceLine() = default;
    ~FCGeoReferenceLine() override = default;
    FCGeoEnum::FITKDatumType getDatumType() override;
};

/**
 * @brief 有限线段
 */
class FCGEOMETRYINTERFACE_API FCGeoReferenceSegment : public FCGeoReferenceLine
{
    FC_CLASS(FC, FCGeoReferenceSegment);
public:
    FCGeoReferenceSegment() = default;
    ~FCGeoReferenceSegment() override = default;
    FCGeoEnum::FITKDatumType getDatumType() override;

    std::array<double, 3> startPoint() const { return { _pos[0], _pos[1], _pos[2] }; }
    void setStartPoint(double x, double y, double z);
    void setStartPoint(std::array<double, 3> xyz);
    std::array<double, 3> endPoint() const { return { _pos2[0], _pos2[1], _pos2[2] }; }
    void setEndPoint(double x, double y, double z);
    void setEndPoint(std::array<double, 3> xyz);
};

/**
 * @brief 平面相交�?
 */
class FCGEOMETRYINTERFACE_API FCGeoReferenceLineByIntersectPlanes : public FCGeoReferenceLine
{
    FC_CLASS(FC, FCGeoReferenceLineByIntersectPlanes);
public:
    FCGeoReferenceLineByIntersectPlanes() = default;
    ~FCGeoReferenceLineByIntersectPlanes() override = default;
    FCGeoEnum::FITKDatumType getDatumType() override;

    void setPlane1(const QVector<double>& plane) { m_plane1 = plane; }
    QVector<double> plane1() const { return m_plane1; }
    void setPlane2(const QVector<double>& plane) { m_plane2 = plane; }
    QVector<double> plane2() const { return m_plane2; }
protected:
    QVector<double> m_plane1;
    QVector<double> m_plane2;
};

/**
 * @brief 旋转�?
 */
class FCGEOMETRYINTERFACE_API FCGeoReferenceLineByRotateLine : public FCGeoReferenceLine
{
    FC_CLASS(FC, FCGeoReferenceLineByRotateLine);
public:
    FCGeoReferenceLineByRotateLine() = default;
    ~FCGeoReferenceLineByRotateLine() override = default;
    FCGeoEnum::FITKDatumType getDatumType() override;

    void setLine(const QVector<double>& start, const QVector<double>& end);
    std::array<double, 3> lineStart() const { return m_lineStart; }
    std::array<double, 3> lineEnd() const { return m_lineEnd; }
    void setAxis(const QVector<double>& start, const QVector<double>& end);
    std::array<double, 3> axisStart() const { return m_axisStart; }
    std::array<double, 3> axisEnd() const { return m_axisEnd; }
    void setAngle(double angle) { m_angle = angle; }
    double angle() const { return m_angle; }
protected:
    std::array<double, 3> m_lineStart{};
    std::array<double, 3> m_lineEnd{};
    std::array<double, 3> m_axisStart{};
    std::array<double, 3> m_axisEnd{};
    double m_angle{};
};

} // namespace FC

#endif // FCGEOREFERENCELINE_H
