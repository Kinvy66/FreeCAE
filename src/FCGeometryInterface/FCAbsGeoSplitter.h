/**
 * @file FCGeoSplitter.h
 * @brief 模型分割命令（移植自 FITKInterfaceGeometry FITKAbsGeoSplitter）
 */
#ifndef FCGEOSPLITTER_H
#define FCGEOSPLITTER_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include <array>

namespace FC {

/**
 * @brief 分割基类
 */
class FCGEOMETRYINTERFACE_API FCGeoSplitter : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoSplitter);
public:
    FCGeoSplitter() = default;
    ~FCGeoSplitter() override = default;

    VirtualShape sourceShape() const { return m_sourceShape; }
    void setSourceShape(const VirtualShape& shape) { m_sourceShape = shape; }
    VirtualShape toolShape() const { return m_toolShape; }
    void setToolShape(const VirtualShape& shape) { m_toolPoint = {}; m_toolShape = shape; }
    std::array<double, 3> toolPoint() const { return m_toolPoint; }
    void setToolPoint(std::array<double, 3> point) { m_toolShape = VirtualShape(); m_toolPoint = point; }
protected:
    VirtualShape m_sourceShape{};
    VirtualShape m_toolShape{};
    std::array<double, 3> m_toolPoint{};
};

/**
 * @brief 曲线分割命令
 */
class FCGEOMETRYINTERFACE_API FCGeoCurveSplitter : public FCGeoSplitter
{
    FC_CLASS(FC, FCGeoCurveSplitter);
public:
    FCGeoCurveSplitter() = default;
    ~FCGeoCurveSplitter() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;

    std::array<double, 3> point() const { return m_point; }
    void setPoint(std::array<double, 3> xyz) { m_point = xyz; }
    void setPoint(double x, double y, double z) { m_point = { x, y, z }; }
protected:
    std::array<double, 3> m_point{};
};

/**
 * @brief 面分割命令
 */
class FCGEOMETRYINTERFACE_API FCGeoSurfaceSplitter : public FCGeoSplitter
{
    FC_CLASS(FC, FCGeoSurfaceSplitter);
public:
    FCGeoSurfaceSplitter() = default;
    ~FCGeoSurfaceSplitter() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

/**
 * @brief 体分割命令
 */
class FCGEOMETRYINTERFACE_API FCGeoSolidSplitter : public FCGeoSplitter
{
    FC_CLASS(FC, FCGeoSolidSplitter);
public:
    FCGeoSolidSplitter() = default;
    ~FCGeoSolidSplitter() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace FC

#endif // FCGEOSPLITTER_H
