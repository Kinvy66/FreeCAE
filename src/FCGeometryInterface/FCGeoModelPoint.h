/**
 * @file FCGeoModelPoint.h
 * @brief 点几何命令接口（移植自 FITKInterfaceGeometry FITKAbsGeoModelPoint）
 */
#ifndef FCGEOMODELPOINT_H
#define FCGEOMODELPOINT_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"

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

} // namespace FC

#endif // FCGEOMODELPOINT_H
