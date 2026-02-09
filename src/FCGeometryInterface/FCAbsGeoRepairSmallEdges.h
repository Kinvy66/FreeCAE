/**
 * @file FCGeoRepairSmallEdges.h
 * @brief 修复小边命令（移植自 FITKInterfaceGeometry FITKAbsGeoRepairSmallEdges）
 */
#ifndef FCGEOREPAIRSMALLEDGES_H
#define FCGEOREPAIRSMALLEDGES_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include <QList>

namespace FC {

/**
 * @brief 修复小边命令
 */
class FCGEOMETRYINTERFACE_API FCGeoRepairSmallEdges : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoRepairSmallEdges);
public:
    FCGeoRepairSmallEdges() = default;
    ~FCGeoRepairSmallEdges() override = default;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;

    QList<VirtualShape> edges() const { return m_edges; }
    void setEdges(const QList<VirtualShape>& edges) { m_edges = edges; }
protected:
    QList<VirtualShape> m_edges;
};

} // namespace FC

#endif // FCGEOREPAIRSMALLEDGES_H
