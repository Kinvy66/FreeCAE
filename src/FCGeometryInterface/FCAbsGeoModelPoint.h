/**
 * @file FCGeoModelPoint.h
 * @brief 点几何命令接口（移植自 FITKInterfaceGeometry FITKAbsGeoModelPoint）
 */
#ifndef FCGEOMODELPOINT_H
#define FCGEOMODELPOINT_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include <QVector>
#include <array>

namespace FC {

/**
 * @brief 坐标点
 */
class FCGEOMETRYINTERFACE_API FCGeoModelPoint : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoModelPoint);
public:
    FCGeoModelPoint() = default;
    explicit FCGeoModelPoint(double x, double y, double z);
    ~FCGeoModelPoint() override = default;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;

    double x() const { return m_x; }
    void setX(double x) { m_x = x; }
    double y() const { return m_y; }
    void setY(double y) { m_y = y; }
    double z() const { return m_z; }
    void setZ(double z) { m_z = z; }
    void setCoord(double x, double y, double z);

protected:
    double m_x{ 0 };
    double m_y{ 0 };
    double m_z{ 0 };
};

/**
 * @brief 曲线的端点
 */
class FCGEOMETRYINTERFACE_API FCGeoModelCurveEnd : public FCGeoModelPoint
{
    FC_CLASS(FC, FCGeoModelCurveEnd);
public:
    FCGeoModelCurveEnd() = default;
    ~FCGeoModelCurveEnd() override = default;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;

    VirtualShape sourceCurve() const { return m_sourceCurve; }
    void setSourceCurve(const VirtualShape& curve) { m_sourceCurve = curve; }
    bool useStartEnd() const { return m_useStartEnd; }
    void setUseStartEnd(bool use) { m_useStartEnd = use; }
protected:
    VirtualShape m_sourceCurve{};
    bool m_useStartEnd{ true };
};

/**
 * @brief 距离曲线的端点指定距离
 */
class FCGEOMETRYINTERFACE_API FCGeoModelCurveEndDistance : public FCGeoModelPoint
{
    FC_CLASS(FC, FCGeoModelCurveEndDistance);
public:
    FCGeoModelCurveEndDistance() = default;
    ~FCGeoModelCurveEndDistance() override = default;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;

    VirtualShape sourceCurve() const { return m_sourceCurve; }
    void setSourceCurve(const VirtualShape& curve) { m_sourceCurve = curve; }
    bool useStartEnd() const { return m_useStartEnd; }
    void setUseStartEnd(bool use) { m_useStartEnd = use; }
    double distance() const { return m_distance; }
    void setDistance(double d) { m_distance = d; }
protected:
    VirtualShape m_sourceCurve{};
    bool m_useStartEnd{ true };
    double m_distance{};
};

/**
 * @brief 指定曲线比例创建点
 */
class FCGEOMETRYINTERFACE_API FCGeoModelCurveRadio : public FCGeoModelPoint
{
    FC_CLASS(FC, FCGeoModelCurveRadio);
public:
    FCGeoModelCurveRadio() = default;
    ~FCGeoModelCurveRadio() override = default;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;

    VirtualShape sourceCurve() const { return m_sourceCurve; }
    void setSourceCurve(const VirtualShape& curve) { m_sourceCurve = curve; }
    double radio() const { return m_radio; }
    void setRadio(double r) { m_radio = r; }
protected:
    VirtualShape m_sourceCurve{};
    double m_radio{};
};

/**
 * @brief 在线、面上交互生成点
 */
class FCGEOMETRYINTERFACE_API FCGeoModelInteractionPoint : public FCGeoModelPoint
{
    FC_CLASS(FC, FCGeoModelInteractionPoint);
public:
    FCGeoModelInteractionPoint() = default;
    ~FCGeoModelInteractionPoint() override = default;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;

    VirtualShape source() const { return m_source; }
    void setSource(const VirtualShape& s) { m_source = s; }
    int type() const { return m_type; }
    void setType(int t) { m_type = t; }
protected:
    VirtualShape m_source{};
    int m_type{};
};

/**
 * @brief 点在线上的投影
 */
class FCGEOMETRYINTERFACE_API FCGeoModelPointProjectionOnCurve : public FCGeoModelPoint
{
    FC_CLASS(FC, FCGeoModelPointProjectionOnCurve);
public:
    FCGeoModelPointProjectionOnCurve() = default;
    ~FCGeoModelPointProjectionOnCurve() override = default;
    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
    VirtualShape sourcePoint() const { return m_sourcePoint; }
    void setSourcePoint(const VirtualShape& s) { m_sourcePoint = s; }
    VirtualShape sourceCurve() const { return m_sourceCurve; }
    void setSourceCurve(const VirtualShape& s) { m_sourceCurve = s; }
    QVector<std::array<double, 3>> getResultPoints() const { return m_projections; }
    void setResultPoints(const QVector<std::array<double, 3>>& p) { m_projections = p; }
protected:
    VirtualShape m_sourcePoint{};
    VirtualShape m_sourceCurve{};
    QVector<std::array<double, 3>> m_projections;
};

/**
 * @brief 两条线的交点
 */
class FCGEOMETRYINTERFACE_API FCGeoModelTwoCurveIntersectPoint : public FCGeoModelPoint
{
    FC_CLASS(FC, FCGeoModelTwoCurveIntersectPoint);
public:
    FCGeoModelTwoCurveIntersectPoint() = default;
    ~FCGeoModelTwoCurveIntersectPoint() override = default;
    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
    VirtualShape sourceCurve1() const { return m_sourceCurve1; }
    VirtualShape sourceCurve2() const { return m_sourceCurve2; }
    void setSourceCurve(const VirtualShape& c1, const VirtualShape& c2) { m_sourceCurve1 = c1; m_sourceCurve2 = c2; }
    QVector<std::array<double, 3>> getResultPoints() const { return m_intersections; }
    void setResultPoints(const QVector<std::array<double, 3>>& p) { m_intersections = p; }
    bool isNearestPoint() const { return m_isNearestPoint; }
    void setNearestPoint(bool b) { m_isNearestPoint = b; }
protected:
    VirtualShape m_sourceCurve1{};
    VirtualShape m_sourceCurve2{};
    QVector<std::array<double, 3>> m_intersections;
    bool m_isNearestPoint{ false };
};

/**
 * @brief 点在面上的投影
 */
class FCGEOMETRYINTERFACE_API FCGeoModelPointProjectionOnSurface : public FCGeoModelPoint
{
    FC_CLASS(FC, FCGeoModelPointProjectionOnSurface);
public:
    FCGeoModelPointProjectionOnSurface() = default;
    ~FCGeoModelPointProjectionOnSurface() override = default;
    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
    VirtualShape sourcePoint() const { return m_sourcePoint; }
    void setSourcePoint(const VirtualShape& s) { m_sourcePoint = s; }
    VirtualShape sourceSurface() const { return m_sourceSurface; }
    void setSourceSurface(const VirtualShape& s) { m_sourceSurface = s; }
    QVector<std::array<double, 3>> getResultPoints() const { return m_projections; }
    void setResultPoints(const QVector<std::array<double, 3>>& p) { m_projections = p; }
protected:
    VirtualShape m_sourcePoint{};
    VirtualShape m_sourceSurface{};
    QVector<std::array<double, 3>> m_projections;
};

/**
 * @brief 线和面的交点
 */
class FCGEOMETRYINTERFACE_API FCGeoModelCurveIntersectSurface : public FCGeoModelPoint
{
    FC_CLASS(FC, FCGeoModelCurveIntersectSurface);
public:
    FCGeoModelCurveIntersectSurface() = default;
    ~FCGeoModelCurveIntersectSurface() override = default;
    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
    VirtualShape sourceCurve() const { return m_sourceCurve; }
    void setSourceCurve(const VirtualShape& s) { m_sourceCurve = s; }
    VirtualShape sourceSurface() const { return m_sourceSurface; }
    void setSourceSurface(const VirtualShape& s) { m_sourceSurface = s; }
    QVector<std::array<double, 3>> getResultPoints() const { return m_intersections; }
    void setResultPoints(const QVector<std::array<double, 3>>& p) { m_intersections = p; }
protected:
    VirtualShape m_sourceCurve{};
    VirtualShape m_sourceSurface{};
    QVector<std::array<double, 3>> m_intersections;
};

} // namespace FC

#endif // FCGEOMODELPOINT_H
