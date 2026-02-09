/**
 * @file FCGeoModelImportFlattenAssembly.h
 * @brief 拆分扁平装配模式导入（移Щ自 FITKInterfaceGeometry FITKAbsGeoModelImportFlattenAssembly）
 */
#ifndef FCGEOMODELIMPORTFLATTENASSEMBLY_H
#define FCGEOMODELIMPORTFLATTENASSEMBLY_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoImportBase.h"

namespace FC {

/**
 * @brief 拆分扁平装配模式璇诲彇几何
 */
class FCGEOMETRYINTERFACE_API FCGeoModelImportFlattenAssembly : public FCGeoImportBase
{
    FC_CLASS(FC, FCGeoModelImportFlattenAssembly);
public:
    explicit FCGeoModelImportFlattenAssembly() = default;
    ~FCGeoModelImportFlattenAssembly() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace FC

#endif // FCGEOMODELIMPORTFLATTENASSEMBLY_H
