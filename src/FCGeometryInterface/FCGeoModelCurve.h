/**
 * @file FCGeoModelCurve.h
 * @brief 曲线几何命令接口（移植自 FITKInterfaceGeometry FITKAbsGeoModelCurve，接口层）
 */
#ifndef FCGEOMODELCURVE_H
#define FCGEOMODELCURVE_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include <array>
#include <QList>

namespace FC {

/**
 * @brief 曲线抽象基类（局部坐标系接口）
 */
class FCGEOMETRYINTERFACE_API FCGeoModelCurve : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoModelCurve);
public:
    FCGeoModelCurve() = default;
    ~FCGeoModelCurve() override = default;

    std::array<double, 3> origin() const { return m_origin; }
    void setOrigin(double x, double y, double z);
    void setOrigin(std::array<double, 3> xyz) { m_origin = xyz; }
    std::array<double, 3> xDirection() const { return m_xDirection; }
    std::array<double, 3> yDirection() const { return m_yDirection; }
    void setYDirection(std::array<double, 3> xyz);
    void setYDirection(double x, double y, double z);
    std::array<double, 3> zDirection() const { return m_zDirection; }
    void setZDirection(std::array<double, 3> xyz);
    void setZDirection(double x, double y, double z);
protected:
    std::array<double, 3> m_origin{ { 0, 0, 0 } };
    std::array<double, 3> m_xDirection{ { 1, 0, 0 } };
    std::array<double, 3> m_yDirection{ { 0, 1, 0 } };
    std::array<double, 3> m_zDirection{ { 0, 0, 1 } };
};

/**
 * @brief 无限直线
 */
class FCGEOMETRYINTERFACE_API FCGeoModelLine : public FCGeoModelCurve
{
    FC_CLASS(FC, FCGeoModelLine);
public:
    FCGeoModelLine() = default;
    ~FCGeoModelLine() override = default;
    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
    std::array<double, 3> dir() const { return m_dir; }
    void setDir(double x, double y, double z);
protected:
    std::array<double, 3> m_dir{};
};

/**
 * @brief 有限线段
 */
class FCGEOMETRYINTERFACE_API FCGeoModelSegment : public FCGeoModelCurve
{
    FC_CLASS(FC, FCGeoModelSegment);
public:
    FCGeoModelSegment() = default;
    ~FCGeoModelSegment() override = default;
    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
    std::array<double, 3> startPoint() const { return m_startPoint; }
    void setStartPoint(double x, double y, double z);
    void setStartPoint(std::array<double, 3> xyz) { m_startPoint = xyz; }
    std::array<double, 3> endPoint() const { return m_endPoint; }
    void setEndPoint(double x, double y, double z);
    void setEndPoint(std::array<double, 3> xyz) { m_endPoint = xyz; }
protected:
    std::array<double, 3> m_startPoint{};
    std::array<double, 3> m_endPoint{};
};

/**
 * @brief 线串（多段线）
 */
class FCGEOMETRYINTERFACE_API FCGeoModelWire : public FCGeoModelCurve
{
    FC_CLASS(FC, FCGeoModelWire);
public:
    FCGeoModelWire() = default;
    ~FCGeoModelWire() override = default;
    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
    void setCurves(const QList<VirtualShape>& curves) { m_curves = curves; }
    QList<VirtualShape> curves() const { return m_curves; }
protected:
    QList<VirtualShape> m_curves;
};

/**
 * @brief 圆
 */
class FCGEOMETRYINTERFACE_API FCGeoModelCircle : public FCGeoModelCurve
{
    FC_CLASS(FC, FCGeoModelCircle);
public:
    FCGeoModelCircle() = default;
    ~FCGeoModelCircle() override = default;
    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
    std::array<double, 3> center() const { return m_center; }
    void setCenter(double x, double y, double z);
    void setCenter(std::array<double, 3> xyz) { m_center = xyz; }
    double radius() const { return m_radius; }
    void setRadius(double r) { m_radius = r; }
    std::array<double, 3> normal() const { return m_normal; }
    void setNormal(std::array<double, 3> n) { m_normal = n; }
protected:
    std::array<double, 3> m_center{};
    double m_radius{};
    std::array<double, 3> m_normal{ { 0, 0, 1 } };
};

} // namespace FC

#endif // FCGEOMODELCURVE_H
