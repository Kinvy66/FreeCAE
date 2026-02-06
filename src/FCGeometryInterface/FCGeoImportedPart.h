/**
 * @file FCGeoImportedPart.h
 * @brief 导入的模型部件命令（移植自 FITKInterfaceGeometry FITKAbsGeoImportedPart）
 */
#ifndef FCGEOIMPORTEDPART_H
#define FCGEOIMPORTEDPART_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"

namespace FC {

/**
 * @brief 导入的模型部件命令
 */
class FCGEOMETRYINTERFACE_API FCGeoImportedPart : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoImportedPart);
public:
    FCGeoImportedPart() = default;
    ~FCGeoImportedPart() override = default;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace FC

#endif // FCGEOIMPORTEDPART_H
