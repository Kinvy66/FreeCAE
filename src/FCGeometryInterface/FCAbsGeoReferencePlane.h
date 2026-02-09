/**
 * @file FCGeoReferencePlane.h
  * @brief 参考面命令（移植自 FITKInterfaceGeometry FITKAbsGeoReferencePlane）
 */
#ifndef FCGEOREFERENCEPLANE_H
#define FCGEOREFERENCEPLANE_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoDatum.h"
#include <array>
#include <QVector>

namespace FC {

/**
  * @brief 参考平面
 */
class FCGEOMETRYINTERFACE_API FCGeoReferencePlane : public FCAbsGeoDatumPlane
{
    FC_CLASS(FC, FCGeoReferencePlane);
public:
    FCGeoReferencePlane() = default;
    ~FCGeoReferencePlane() override = default;

    void setPosition(double x, double y, double z);
    void setNormal(double x, double y, double z);
    void setUp(double x, double y, double z);
};

/**
 * @brief 偏移参考面
 */
class FCGEOMETRYINTERFACE_API FCGeoReferenceOffsetPlane : public FCGeoReferencePlane
{
    FC_CLASS(FC, FCGeoReferenceOffsetPlane);
public:
    FCGeoReferenceOffsetPlane() = default;
    ~FCGeoReferenceOffsetPlane() override = default;
    FCGeoEnum::FCDatumType getDatumType() override;

    VirtualShape sourceSurface() const { return m_sourceSurface; }
    void setSourceSurface(const VirtualShape& s) { m_sourceSurface = s; }
    std::array<double, 3> sourcePlaneOrigin() const { return m_sourcePlaneOrigin; }
    void setSourcePlaneOrigin(std::array<double, 3> ori) { m_sourcePlaneOrigin = ori; }
    void setSourcePlaneOrigin(double x, double y, double z) { m_sourcePlaneOrigin = { x, y, z }; }
    std::array<double, 3> sourcePlaneNormal() const { return m_sourcePlaneNormal; }
    void setSourcePlaneNormal(std::array<double, 3> n) { m_sourcePlaneNormal = n; }
    void setSourcePlaneNormal(double x, double y, double z) { m_sourcePlaneNormal = { x, y, z }; }
    std::array<double, 3> sourcePlaneUp() const { return m_sourcePlaneUp; }
    void setSourcePlaneUp(std::array<double, 3> u) { m_sourcePlaneUp = u; }
    void setSourcePlaneUp(double x, double y, double z) { m_sourcePlaneUp = { x, y, z }; }
    std::array<double, 3> direction() const { return m_direction; }
    void setDirection(std::array<double, 3> dir) { m_direction = dir; }
    void setDirection(double x, double y, double z) { m_direction = { x, y, z }; }
    double offset() const { return m_offset; }
    void setOffset(double o) { m_offset = o; }
protected:
    VirtualShape m_sourceSurface{};
    std::array<double, 3> m_sourcePlaneOrigin{};
    std::array<double, 3> m_sourcePlaneNormal{};
    std::array<double, 3> m_sourcePlaneUp{};
    std::array<double, 3> m_direction{};
    double m_offset{};
};

/**
 * @brief 三点创建参考面
 */
class FCGEOMETRYINTERFACE_API FCGeoReferenceThreePointsPlane : public FCGeoReferencePlane
{
    FC_CLASS(FC, FCGeoReferenceThreePointsPlane);
public:
    FCGeoReferenceThreePointsPlane() = default;
    ~FCGeoReferenceThreePointsPlane() override = default;
    FCGeoEnum::FCDatumType getDatumType() override;

    std::array<double, 3> point(int index) const;
    void setPoint(int index, std::array<double, 3> xyz);
    void setPoint(int index, double x, double y, double z);
protected:
    std::array<std::array<double, 3>, 3> m_points{};
};

/**
 * @brief 方程（Ax+By+Cz+D=0）创建参考面
 */
class FCGEOMETRYINTERFACE_API FCGeoReferenceEquationPlane : public FCGeoReferencePlane
{
    FC_CLASS(FC, FCGeoReferenceEquationPlane);
public:
    FCGeoReferenceEquationPlane() = default;
    ~FCGeoReferenceEquationPlane() override = default;
    FCGeoEnum::FCDatumType getDatumType() override;

    double a() const { return m_a; }
    void setA(double val) { m_a = val; }
    double b() const { return m_b; }
    void setB(double val) { m_b = val; }
    double c() const { return m_c; }
    void setC(double val) { m_c = val; }
    double d() const { return m_d; }
    void setD(double val) { m_d = val; }
protected:
    double m_a{};
    double m_b{};
    double m_c{};
    double m_d{};
};

/**
 * @brief 点和法线创建参考面
 */
class FCGEOMETRYINTERFACE_API FCGeoReferencePointAndDirectionPlane : public FCGeoReferencePlane
{
    FC_CLASS(FC, FCGeoReferencePointAndDirectionPlane);
public:
    FCGeoReferencePointAndDirectionPlane() = default;
    ~FCGeoReferencePointAndDirectionPlane() override = default;
    FCGeoEnum::FCDatumType getDatumType() override;

    std::array<double, 3> point() const { return m_point; }
    void setPoint(std::array<double, 3> xyz) { m_point = xyz; }
    void setPoint(double x, double y, double z) { m_point = { x, y, z }; }
    std::array<double, 3> direction() const { return m_direction; }
    void setDirection(std::array<double, 3> xyz) { m_direction = xyz; }
    void setDirection(double x, double y, double z) { m_direction = { x, y, z }; }
protected:
    std::array<double, 3> m_point{};
    std::array<double, 3> m_direction{};
};

/**
 * @brief 旋转已有面创建参考面
 */
class FCGEOMETRYINTERFACE_API FCGeoReferenceRotateFromPlanePlane : public FCGeoReferencePlane
{
    FC_CLASS(FC, FCGeoReferenceRotateFromPlanePlane);
public:
    FCGeoReferenceRotateFromPlanePlane() = default;
    ~FCGeoReferenceRotateFromPlanePlane() override = default;
    FCGeoEnum::FCDatumType getDatumType() override;

    void setPlane(const QVector<double>& plane);
    std::array<double, 3> planePosition() const { return m_planePos; }
    std::array<double, 3> planeNormal() const { return m_planeNor; }
    std::array<double, 3> planeUp() const { return m_planeUp; }
    void setAxis(const QVector<double>& start, const QVector<double>& end);
    std::array<double, 3> axisStart() const { return m_axisStart; }
    std::array<double, 3> axisEnd() const { return m_axisEnd; }
    void setAngle(double angle) { m_angle = angle; }
    double angle() const { return m_angle; }
protected:
    std::array<double, 3> m_planePos{};
    std::array<double, 3> m_planeNor{};
    std::array<double, 3> m_planeUp{};
    std::array<double, 3> m_axisStart{};
    std::array<double, 3> m_axisEnd{};
    double m_angle{};
};

} // namespace FC

#endif // FCGEOREFERENCEPLANE_H
