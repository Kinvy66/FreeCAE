/**
 * @file FCGeoStitch.h
 * @brief 缝合面命令（移植自 FITKInterfaceGeometry FITKAbsGeoStitch）
 */
#ifndef FCGEOSTITCH_H
#define FCGEOSTITCH_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include <QList>

namespace FC {

/**
 * @brief 缝合面命令
 */
class FCGEOMETRYINTERFACE_API FCGeoStitch : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoStitch);
public:
    FCGeoStitch() = default;
    ~FCGeoStitch() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;

    double tolerance() const { return m_tolerance; }
    void setTolerance(double tol) { m_tolerance = tol; }
    QList<VirtualShape> edges() const { return m_edges; }
    void setEdges(const QList<VirtualShape>& edges) { m_edges = edges; }
    /** 基准形状来源（提供待缝合的体/壳）；若无效则仅对 edges 做缝合 */
    void setInputCmdId(const VirtualShape& vs) { m_inputCmdId = vs; }
    const VirtualShape& getInputCmdId() const { return m_inputCmdId; }
protected:
    double m_tolerance{};
    QList<VirtualShape> m_edges;
    VirtualShape m_inputCmdId{};
};

} // namespace FC

#endif // FCGEOSTITCH_H
