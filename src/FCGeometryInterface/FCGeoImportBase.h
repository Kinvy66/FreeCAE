/**
 * @file FCGeoImportBase.h
 * @brief 几何导入命令抽象基类（移植自 FITKInterfaceGeometry FITKAbsGeoImportBase）
 */
#ifndef FCGEOIMPORTBASE_H
#define FCGEOIMPORTBASE_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include <QString>

namespace FC {

/**
 * @brief 几何导入命令抽象基类，子类实现 getGeometryCommandType/update
 */
class FCGEOMETRYINTERFACE_API FCGeoImportBase : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoImportBase);
public:
    explicit FCGeoImportBase() = default;
    ~FCGeoImportBase() override = default;

    void setFileName(const QString& fileName);
    QString getFileName() const;

protected:
    QString _importFileName{};
};

} // namespace FC

#endif // FCGEOIMPORTBASE_H
