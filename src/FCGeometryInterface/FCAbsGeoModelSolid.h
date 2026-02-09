/**
 * @file FCAbsGeoModelSolid.h
 * @brief 实体几何命令接口（移植自 FITKInterfaceGeometry FITKAbsGeoModelSolid）
 */
#ifndef FCABSGEOMODELSOLID_H
#define FCABSGEOMODELSOLID_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include <QList>
#include <array>

namespace FC {

/**
 * @brief 实体抽象基类（抽象）
 */
class FCGEOMETRYINTERFACE_API FCAbsGeoModelSolid : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCAbsGeoModelSolid);
public:
    FCAbsGeoModelSolid() = default;
    ~FCAbsGeoModelSolid() override = default;
};

/**
 * @brief 封闭曲面形成的体
 */
class FCGEOMETRYINTERFACE_API FCAbsGeoModelClosedSurfaceSolid : public FCAbsGeoModelSolid
{
    FC_CLASS(FC, FCAbsGeoModelClosedSurfaceSolid);
public:
    FCAbsGeoModelClosedSurfaceSolid() = default;
    ~FCAbsGeoModelClosedSurfaceSolid() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;

    void addFace(const VirtualShape& face) { m_faces.append(face); }
    void setFaces(const QList<VirtualShape>& faces) { m_faces = faces; }
    QList<VirtualShape> faces() const { return m_faces; }
protected:
    QList<VirtualShape> m_faces;
};

/**
 * @brief 拉伸实体
 */
class FCGEOMETRYINTERFACE_API FCAbsGeoModelExtrudeSolid : public FCAbsGeoModelSolid
{
    FC_CLASS(FC, FCAbsGeoModelExtrudeSolid);
public:
    FCAbsGeoModelExtrudeSolid() = default;
    ~FCAbsGeoModelExtrudeSolid() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;

    VirtualShape sourceSurface() const { return m_sourceSurface; }
    void setSourceSurface(const VirtualShape& s) { m_sourceSurface = s; }
    VirtualShape upToFace() const { return m_upToFace; }
    void setUpToFace(const VirtualShape& s) { m_upToFace = s; }
    std::array<double, 3> direction() const { return m_direction; }
    void setDirection(double x, double y, double z) { m_direction = { x, y, z }; }
    void setDirection(std::array<double, 3> xyz) { m_direction = xyz; }
    double length() const { return m_length; }
    void setLength(double len) { m_length = len; }
    int extrudeType() const { return m_extrudeType; }
    void setExtrudeType(int type) { m_extrudeType = type; }
protected:
    VirtualShape m_sourceSurface{};
    VirtualShape m_upToFace{};
    std::array<double, 3> m_direction{};
    double m_length{};
    int m_extrudeType{ 1 };
};

/**
 * @brief 旋转实体
 */
class FCGEOMETRYINTERFACE_API FCAbsGeoModelRevolSolid : public FCAbsGeoModelSolid
{
    FC_CLASS(FC, FCAbsGeoModelRevolSolid);
public:
    FCAbsGeoModelRevolSolid() = default;
    ~FCAbsGeoModelRevolSolid() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;

    VirtualShape sourceSurface() const { return m_sourceSurface; }
    void setSourceSurface(const VirtualShape& s) { m_sourceSurface = s; }
    std::array<double, 3> rotateAxisPoint1() const { return m_rotateAxisPoint1; }
    void setRotateAxisPoint1(double x, double y, double z) { m_rotateAxisPoint1 = { x, y, z }; }
    void setRotateAxisPoint1(std::array<double, 3> xyz) { m_rotateAxisPoint1 = xyz; }
    std::array<double, 3> rotateAxisPoint2() const { return m_rotateAxisPoint2; }
    void setRotateAxisPoint2(double x, double y, double z) { m_rotateAxisPoint2 = { x, y, z }; }
    void setRotateAxisPoint2(std::array<double, 3> xyz) { m_rotateAxisPoint2 = xyz; }
    double angle() const { return m_angle; }
    void setAngle(double degree) { m_angle = degree; }
protected:
    VirtualShape m_sourceSurface{};
    std::array<double, 3> m_rotateAxisPoint1{};
    std::array<double, 3> m_rotateAxisPoint2{};
    double m_angle{};
};

/**
 * @brief 扫掠实体命令
 */
class FCGEOMETRYINTERFACE_API FCAbsGeoModelSweepSolid : public FCAbsGeoModelSolid
{
    FC_CLASS(FC, FCAbsGeoModelSweepSolid);
public:
    FCAbsGeoModelSweepSolid() = default;
    ~FCAbsGeoModelSweepSolid() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;

    VirtualShape profile() const { return m_profile; }
    void setProfile(const VirtualShape& s) { m_profile = s; }
    VirtualShape curve() const { return m_curves.isEmpty() ? VirtualShape() : m_curves.first(); }
    void setCurve(const VirtualShape& s) { m_curves = { s }; }
    QList<VirtualShape> curves() const { return m_curves; }
    void setCurves(const QList<VirtualShape>& shapes) { m_curves = shapes; }
protected:
    VirtualShape m_profile{};
    QList<VirtualShape> m_curves;
};

/**
 * @brief 多截面扫掠体
 */
class FCGEOMETRYINTERFACE_API FCAbsGeoModelMultiSectionSolid : public FCAbsGeoModelSolid
{
    FC_CLASS(FC, FCAbsGeoModelMultiSectionSolid);
public:
    FCAbsGeoModelMultiSectionSolid() = default;
    ~FCAbsGeoModelMultiSectionSolid() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;

    QList<VirtualShape> sections() const { return m_sections; }
    VirtualShape section(int index) const;
    void setSection(int index, const VirtualShape& s);
    void setSections(const QList<VirtualShape>& sections) { m_sections = sections; }
    void addSection(const VirtualShape& s) { m_sections.append(s); }
protected:
    QList<VirtualShape> m_sections;
};

} // namespace FC

#endif // FCABSGEOMODELSOLID_H

