/**
 * @file FCGeoModelExport.h
 * @brief 几何导出命令接口（移植自 FITKInterfaceGeometry FITKAbsGeoModelExport）
 */
#ifndef FCGEOMODELEXPORT_H
#define FCGEOMODELEXPORT_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include <QList>
#include <QString>

namespace FC {

/**
 * @brief 几何导出命令
 */
class FCGEOMETRYINTERFACE_API FCGeoModelExport : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoModelExport);
public:
    explicit FCGeoModelExport() = default;
    ~FCGeoModelExport() override = default;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;

    void setFileName(const QString& fileName);
    QString getFileName() const;

    void addExportCommandID(int id);
    void removeExportCommandID(int id);
    QList<int> getExportCommandIDs() const { return _exportIds; }

protected:
    QString _exportFileName{};
    QList<int> _exportIds;
};

} // namespace FC

#endif // FCGEOMODELEXPORT_H
