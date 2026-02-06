/**
 * @file FCGeoRepairSmallFaces.h
 * @brief 修复小面命令（移植自 FITKInterfaceGeometry FITKAbsGeoRepairSmallFaces）
 */
#ifndef FCGEOREPAIRSMALLFACES_H
#define FCGEOREPAIRSMALLFACES_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include <QList>

namespace FC {

/**
 * @brief 修复小面
 */
class FCGEOMETRYINTERFACE_API FCGeoRepairSmallFaces : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoRepairSmallFaces);
public:
    FCGeoRepairSmallFaces() = default;
    ~FCGeoRepairSmallFaces() override = default;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;

    QList<VirtualShape> faces() const { return m_faces; }
    void setFaces(const QList<VirtualShape>& faces) { m_faces = faces; }
protected:
    QList<VirtualShape> m_faces;
};

} // namespace FC

#endif // FCGEOREPAIRSMALLFACES_H
