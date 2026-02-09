/**
 * @file FCAbsGeoInstance.h
 * @brief 装配实例命令（引用部件）（移植自 FITKInterfaceGeometry）
 */
#ifndef FCABSGEOINSTANCE_H
#define FCABSGEOINSTANCE_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include "FCGeoEnum.h"
#include <FCData/FCMacros.h>

namespace FC {

class FCAbsGeoPart;

class FCGEOMETRYINTERFACE_API FCAbsGeoInstance : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCAbsGeoInstance);
public:
    FCAbsGeoInstance() = default;
    ~FCAbsGeoInstance() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;

    void setPartCommandID(int id);
    int getPartCommandID();
    FCAbsGeoPart* getPartCommand();

protected:
    int _partCmdId{ -1 };
};

} // namespace FC

#endif // FCABSGEOINSTANCE_H
