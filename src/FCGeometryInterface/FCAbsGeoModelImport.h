/**
 * @file FCGeoModelImport.h
 * @brief 几何导入命令接口（移Щ自 FITKInterfaceGeometry FITKAbsGeoModelImport）
 */
#ifndef FCGEOMODELIMPORT_H
#define FCGEOMODELIMPORT_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoImportBase.h"

namespace FC {

/**
 * @brief 几何导入命令
 */
class FCGEOMETRYINTERFACE_API FCGeoModelImport : public FCGeoImportBase
{
    FC_CLASS(FC, FCGeoModelImport);
public:
    explicit FCGeoModelImport() = default;
    ~FCGeoModelImport() override = default;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace FC

#endif // FCGEOMODELIMPORT_H
