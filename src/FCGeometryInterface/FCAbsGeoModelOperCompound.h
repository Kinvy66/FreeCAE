/**
 * @file FCGeoModelOperCompound.h
 * @brief 复合体操作命令（移植自 FITKInterfaceGeometry FITKAbsGeoModelOperCompound）
 */
#ifndef FCGEOMODELOPERCOMPOUND_H
#define FCGEOMODELOPERCOMPOUND_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include <QList>

namespace FC {

/**
 * @brief 复合体操作（追加/移除形状）
 */
class FCGEOMETRYINTERFACE_API FCGeoModelOperCompound : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoModelOperCompound);
public:
    FCGeoModelOperCompound() = default;
    ~FCGeoModelOperCompound() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;

    void setInputCmdId(const VirtualShape& cmdId) { m_solidCmdId = cmdId; }
    VirtualShape& getInputCmdId() { return m_solidCmdId; }
    const VirtualShape& getInputCmdId() const { return m_solidCmdId; }
    void setShapeCmdIds(const QList<VirtualShape>& cmdIds) { m_solidCmdIds = cmdIds; }
    QList<VirtualShape>& getShapeCmdIds() { return m_solidCmdIds; }
    const QList<VirtualShape>& getShapeCmdIds() const { return m_solidCmdIds; }
protected:
    VirtualShape m_solidCmdId{};
    QList<VirtualShape> m_solidCmdIds;
};

} // namespace FC

#endif // FCGEOMODELOPERCOMPOUND_H
