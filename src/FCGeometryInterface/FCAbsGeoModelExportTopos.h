/**
 * @file FCGeoModelExportTopos.h
 * @brief 导出几何拓扑命令（移植自 FITKInterfaceGeometry FITKAbsGeoModelExportTopos）
 */
#ifndef FCGEOMODELEXPORTTOPOS_H
#define FCGEOMODELEXPORTTOPOS_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include <QList>
#include <QString>

namespace FC {

/**
 * @brief 导出几何部分形状（拓扑）
 */
class FCGEOMETRYINTERFACE_API FCGeoModelExportTopos : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoModelExportTopos);
public:
    explicit FCGeoModelExportTopos() = default;
    ~FCGeoModelExportTopos() override = default;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;

    void setFileName(const QString& fileName);
    QString getFileName() const;
    void setExportTopos(const QList<VirtualShape>& topos) { _exportTopos = topos; }
    const QList<VirtualShape>& getExportTopos() const { return _exportTopos; }
    void appendExportTopo(const VirtualShape& topo) { _exportTopos.append(topo); }
    void clearExportTopos() { _exportTopos.clear(); }
protected:
    QString _exportFileName;
    QList<VirtualShape> _exportTopos;
};

} // namespace FC

#endif // FCGEOMODELEXPORTTOPOS_H
