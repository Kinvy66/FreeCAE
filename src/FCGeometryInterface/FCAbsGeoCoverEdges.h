/**
 * @file FCGeoCoverEdges.h
 * @brief 覆盖面命令接口（移植自 FITKInterfaceGeometry FITKAbsGeoCoverEdges）
 */
#ifndef FCGEOCOVEREDGES_H
#define FCGEOCOVEREDGES_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include <QList>

namespace FC {

/**
 * @brief 覆盖面命令
 */
class FCGEOMETRYINTERFACE_API FCGeoCoverEdges : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoCoverEdges);
public:
    FCGeoCoverEdges() = default;
    ~FCGeoCoverEdges() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;

    QList<VirtualShape> edges() const { return m_edges; }
    void setEdges(const QList<VirtualShape>& edges) { m_edges = edges; }
protected:
    QList<VirtualShape> m_edges;
};

} // namespace FC

#endif // FCGEOCOVEREDGES_H
