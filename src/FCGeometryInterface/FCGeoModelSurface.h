/**
 * @file FCGeoModelSurface.h
 * @brief 曲面几何命令接口（移植自 FITKInterfaceGeometry FITKAbsGeoModelSurface，接口层）
 */
#ifndef FCGEOMODELSURFACE_H
#define FCGEOMODELSURFACE_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include <array>
#include <QList>

namespace FC {

/**
 * @brief 曲面抽象基类
 */
class FCGEOMETRYINTERFACE_API FCGeoModelSurface : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoModelSurface);
public:
    FCGeoModelSurface() = default;
    ~FCGeoModelSurface() override = default;
};

/**
 * @brief 封闭曲线形成的面
 */
class FCGEOMETRYINTERFACE_API FCGeoModelClosedWireSurface : public FCGeoModelSurface
{
    FC_CLASS(FC, FCGeoModelClosedWireSurface);
public:
    FCGeoModelClosedWireSurface() = default;
    ~FCGeoModelClosedWireSurface() override = default;
    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
    void addEdge(VirtualShape edge) { m_edges.append(edge); }
    void setEdges(const QList<VirtualShape>& edges) { m_edges = edges; }
    QList<VirtualShape> edges() const { return m_edges; }
protected:
    QList<VirtualShape> m_edges;
};

/**
 * @brief 偏移曲面
 */
class FCGEOMETRYINTERFACE_API FCGeoModelOffsetSurface : public FCGeoModelSurface
{
    FC_CLASS(FC, FCGeoModelOffsetSurface);
public:
    FCGeoModelOffsetSurface() = default;
    ~FCGeoModelOffsetSurface() override = default;
    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
    VirtualShape sourceSurface() const { return m_sourceSurface; }
    void setSourceSurface(VirtualShape shape) { m_sourceSurface = shape; }
    double offset() const { return m_offset; }
    void setOffset(double o) { m_offset = o; }
protected:
    VirtualShape m_sourceSurface;
    double m_offset{};
};

/**
 * @brief 拉伸曲面
 */
class FCGEOMETRYINTERFACE_API FCGeoModelExtrudeSurface : public FCGeoModelSurface
{
    FC_CLASS(FC, FCGeoModelExtrudeSurface);
public:
    FCGeoModelExtrudeSurface() = default;
    ~FCGeoModelExtrudeSurface() override = default;
    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
    VirtualShape sourceCurve() const { return m_sourceCurve; }
    void setSourceCurve(VirtualShape shape) { m_sourceCurve = shape; }
    VirtualShape upToFace() const { return m_upToFace; }
    void setUpToFace(VirtualShape shape) { m_upToFace = shape; }
    std::array<double, 3> direction() const { return m_direction; }
    void setDirection(double x, double y, double z) { m_direction = { x, y, z }; }
    void setDirection(std::array<double, 3> xyz) { m_direction = xyz; }
    double length() const { return m_length; }
    void setLength(double len) { m_length = len; }
    int extrudeType() const { return m_extrudeType; }
    void setExtrudeType(int type) { m_extrudeType = type; }
protected:
    VirtualShape m_sourceCurve;
    VirtualShape m_upToFace;
    std::array<double, 3> m_direction{};
    double m_length{};
    int m_extrudeType{ 1 };
};

/**
 * @brief 旋转曲面
 */
class FCGEOMETRYINTERFACE_API FCGeoModelRevolSurface : public FCGeoModelSurface
{
    FC_CLASS(FC, FCGeoModelRevolSurface);
public:
    FCGeoModelRevolSurface() = default;
    ~FCGeoModelRevolSurface() override = default;
    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
    VirtualShape sourceCurve() const { return m_sourceCurve; }
    void setSourceCurve(VirtualShape shape) { m_sourceCurve = shape; }
    std::array<double, 3> rotateAxisPoint1() const { return m_rotateAxisPoint1; }
    void setRotateAxisPoint1(double x, double y, double z) { m_rotateAxisPoint1 = { x, y, z }; }
    void setRotateAxisPoint1(std::array<double, 3> xyz) { m_rotateAxisPoint1 = xyz; }
    std::array<double, 3> rotateAxisPoint2() const { return m_rotateAxisPoint2; }
    void setRotateAxisPoint2(double x, double y, double z) { m_rotateAxisPoint2 = { x, y, z }; }
    void setRotateAxisPoint2(std::array<double, 3> xyz) { m_rotateAxisPoint2 = xyz; }
    double angle() const { return m_angle; }
    void setAngle(double degree) { m_angle = degree; }
protected:
    VirtualShape m_sourceCurve;
    std::array<double, 3> m_rotateAxisPoint1{};
    std::array<double, 3> m_rotateAxisPoint2{};
    double m_angle{};
};

/**
 * @brief 扫略曲面
 */
class FCGEOMETRYINTERFACE_API FCGeoModelSweepSurface : public FCGeoModelSurface
{
    FC_CLASS(FC, FCGeoModelSweepSurface);
public:
    FCGeoModelSweepSurface() = default;
    ~FCGeoModelSweepSurface() override = default;
    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
    VirtualShape profile() const { return m_profiles.isEmpty() ? VirtualShape() : m_profiles.first(); }
    void setProfile(VirtualShape shape) { m_profiles = { shape }; }
    QList<VirtualShape> profiles() const { return m_profiles; }
    void setProfiles(const QList<VirtualShape>& shapes) { m_profiles = shapes; }
    VirtualShape curve() const { return m_curves.isEmpty() ? VirtualShape() : m_curves.first(); }
    void setCurve(VirtualShape shape) { m_curves = { shape }; }
    QList<VirtualShape> curves() const { return m_curves; }
    void setCurves(const QList<VirtualShape>& shapes) { m_curves = shapes; }
protected:
    QList<VirtualShape> m_profiles;
    QList<VirtualShape> m_curves;
};

} // namespace FC

#endif // FCGEOMODELSURFACE_H
