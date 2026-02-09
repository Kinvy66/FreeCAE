/**
 * @file FCGeoImportedPart.h
 * @brief 
 */
#ifndef FCGEOIMPORTEDPART_H
#define FCGEOIMPORTEDPART_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"

namespace FC {

/**
 * @brief 导入几何部件命令
 */
class FCGEOMETRYINTERFACE_API FCGeoImportedPart : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoImportedPart);
public:
    FCGeoImportedPart() = default;
    ~FCGeoImportedPart() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace FC

#endif // FCGEOIMPORTEDPART_H
