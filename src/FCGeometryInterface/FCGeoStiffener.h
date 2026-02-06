/**
 * @file FCGeoStiffener.h
 * @brief 肋特征命令（移植自 FITKInterfaceGeometry FITKAbsGeoStiffener）
 */
#ifndef FCGEOSTIFFENER_H
#define FCGEOSTIFFENER_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"

namespace FC {

/**
 * @brief 肋特征
 */
class FCGEOMETRYINTERFACE_API FCGeoStiffener : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoStiffener);
public:
    FCGeoStiffener() = default;
    ~FCGeoStiffener() override = default;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;

    VirtualShape sourceShape() const { return m_sourceShape; }
    void setSourceShape(const VirtualShape& shape) { m_sourceShape = shape; }
    VirtualShape profileShape() const { return m_profileShape; }
    void setProfileShape(const VirtualShape& shape) { m_profileShape = shape; }
    double thickness1() const { return m_thickness1; }
    void setThickness1(double t) { m_thickness1 = t; }
    double thickness2() const { return m_thickness2; }
    void setThickness2(double t) { m_thickness2 = t; }
protected:
    VirtualShape m_sourceShape{};
    VirtualShape m_profileShape{};
    double m_thickness1{};
    double m_thickness2{};
};

} // namespace FC

#endif // FCGEOSTIFFENER_H
