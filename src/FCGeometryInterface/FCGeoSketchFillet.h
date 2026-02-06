/**
 * @file FCGeoSketchFillet.h
 * @brief 草图倒角命令（移植自 FITKInterfaceGeometry FITKAbsGeoSketchFillet）
 */
#ifndef FCGEOSKETCHFILLET_H
#define FCGEOSKETCHFILLET_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"

namespace FC {

/**
 * @brief 草图倒角
 */
class FCGEOMETRYINTERFACE_API FCGeoSketchFillet : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoSketchFillet);
public:
    FCGeoSketchFillet() = default;
    ~FCGeoSketchFillet() override = default;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;

    VirtualShape firstEdge() const { return m_firstEdge; }
    void setFirstEdge(const VirtualShape& shape) { m_firstEdge = shape; }
    VirtualShape secondEdge() const { return m_secondEdge; }
    void setSecondEdge(const VirtualShape& shape) { m_secondEdge = shape; }
    double radius() const { return m_radius; }
    void setRadius(double r) { m_radius = r; }
protected:
    VirtualShape m_firstEdge{};
    VirtualShape m_secondEdge{};
    double m_radius{};
};

} // namespace FC

#endif // FCGEOSKETCHFILLET_H
