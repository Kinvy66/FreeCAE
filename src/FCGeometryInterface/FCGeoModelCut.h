/**
 * @file FCGeoModelCut.h
 * @brief 实体切割命令接口（移植自 FITKInterfaceGeometry FITKAbsGeoModelCut）
 */
#ifndef FCGEOMODELCUT_H
#define FCGEOMODELCUT_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include <QList>
#include <array>

namespace FC {

/**
 * @brief 实体切割抽象基类
 */
class FCGEOMETRYINTERFACE_API FCGeoModelCut : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoModelCut);
public:
    FCGeoModelCut() = default;
    ~FCGeoModelCut() override = default;
};

/**
 * @brief 拉伸切割
 */
class FCGEOMETRYINTERFACE_API FCGeoModelExtrudeCut : public FCGeoModelCut
{
    FC_CLASS(FC, FCGeoModelExtrudeCut);
public:
    FCGeoModelExtrudeCut() = default;
    ~FCGeoModelExtrudeCut() override = default;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
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
 * @brief 旋转切割
 */
class FCGEOMETRYINTERFACE_API FCGeoModelRevolCut : public FCGeoModelCut
{
    FC_CLASS(FC, FCGeoModelRevolCut);
public:
    FCGeoModelRevolCut() = default;
    ~FCGeoModelRevolCut() override = default;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
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
 * @brief 扫略切割
 */
class FCGEOMETRYINTERFACE_API FCGeoModelSweepCut : public FCGeoModelCut
{
    FC_CLASS(FC, FCGeoModelSweepCut);
public:
    FCGeoModelSweepCut() = default;
    ~FCGeoModelSweepCut() override = default;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;

    VirtualShape profile() const { return m_profile; }
    void setProfile(const VirtualShape& s) { m_profile = s; }
    VirtualShape curve() const { return m_curve; }
    void setCurve(const VirtualShape& s) { m_curve = s; }
    QList<VirtualShape> curves() const { return m_curves; }
    void setCurves(const QList<VirtualShape>& shapes) { m_curves = shapes; }
protected:
    VirtualShape m_profile{};
    VirtualShape m_curve{};
    QList<VirtualShape> m_curves;
};

/**
 * @brief 多截面扫略切割
 */
class FCGEOMETRYINTERFACE_API FCGeoModelMultiSectionCut : public FCGeoModelCut
{
    FC_CLASS(FC, FCGeoModelMultiSectionCut);
public:
    FCGeoModelMultiSectionCut() = default;
    ~FCGeoModelMultiSectionCut() override = default;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
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

#endif // FCGEOMODELCUT_H
