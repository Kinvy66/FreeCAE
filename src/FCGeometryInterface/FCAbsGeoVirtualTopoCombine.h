/**
 * @file FCGeoVirtualTopoCombine.h
 * @brief 虚拓扑合并命令（移植自 FITKInterfaceGeometry FITKAbsGeoVirtualTopoCombine）
 */
#ifndef FCGEOVIRTUALTOPOCOMBINE_H
#define FCGEOVIRTUALTOPOCOMBINE_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include <QList>

namespace FC {

/**
 * @brief 虚拓扑合并基类
 */
class FCGEOMETRYINTERFACE_API FCGeoVirtualTopoCombine : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoVirtualTopoCombine);
public:
    FCGeoVirtualTopoCombine() = default;
    ~FCGeoVirtualTopoCombine() override = default;

    void setCombineTopos(const QList<VirtualShape>& topos) { m_combineTopos = topos; }
    QList<VirtualShape> combineTopos() const { return m_combineTopos; }
    void setResultTopos(const QList<VirtualShape>& topos) { m_resultTopos = topos; }
    QList<VirtualShape> resultTopos() const { return m_resultTopos; }
    void removeFromPart(FCAbsGeoCommand* part) override;
protected:
    QList<VirtualShape> m_combineTopos;
    QList<VirtualShape> m_resultTopos;
};

/**
 * @brief 虚拓扑合并面
 */
class FCGEOMETRYINTERFACE_API FCGeoVirtualTopoCombineFaces : public FCGeoVirtualTopoCombine
{
    FC_CLASS(FC, FCGeoVirtualTopoCombineFaces);
public:
    FCGeoVirtualTopoCombineFaces() = default;
    ~FCGeoVirtualTopoCombineFaces() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;

    void setSkipSharpAngle(bool skip) { m_skipSharpAngle = skip; }
    bool skipSharpAngle() const { return m_skipSharpAngle; }
    void setCombineBounding(bool b) { m_combineBounding = b; }
    bool combineBounding() const { return m_combineBounding; }
protected:
    bool m_skipSharpAngle{ true };
    bool m_combineBounding{ true };
};

/**
 * @brief 虚拓扑合并边
 */
class FCGEOMETRYINTERFACE_API FCGeoVirtualTopoCombineEdges : public FCGeoVirtualTopoCombine
{
    FC_CLASS(FC, FCGeoVirtualTopoCombineEdges);
public:
    FCGeoVirtualTopoCombineEdges() = default;
    ~FCGeoVirtualTopoCombineEdges() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;

    void setSkipSharpAngle(bool skip) { m_skipSharpAngle = skip; }
    bool skipSharpAngle() const { return m_skipSharpAngle; }
protected:
    bool m_skipSharpAngle{ true };
};

} // namespace FC

#endif // FCGEOVIRTUALTOPOCOMBINE_H
