/**
 * @file FCGeoStitch.h
 * @brief 缝合面命令（移植�?FITKInterfaceGeometry FITKAbsGeoStitch�?
 */
#ifndef FCGEOSTITCH_H
#define FCGEOSTITCH_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include <QList>

namespace FC {

/**
 * @brief 缝合�?
 */
class FCGEOMETRYINTERFACE_API FCGeoStitch : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoStitch);
public:
    FCGeoStitch() = default;
    ~FCGeoStitch() override = default;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;

    double tolerance() const { return m_tolerance; }
    void setTolerance(double tol) { m_tolerance = tol; }
    QList<VirtualShape> edges() const { return m_edges; }
    void setEdges(const QList<VirtualShape>& edges) { m_edges = edges; }
protected:
    double m_tolerance{};
    QList<VirtualShape> m_edges;
};

} // namespace FC

#endif // FCGEOSTITCH_H
