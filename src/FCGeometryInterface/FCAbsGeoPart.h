/**
 * @file FCAbsGeoPart.h
 * @brief 部件命令（命令 + 子命令容器）（移植自 FITKInterfaceGeometry）
 */
#ifndef FCABSGEOPART_H
#define FCABSGEOPART_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include "FCGeoEnum.h"
#include <FCData/FCMacros.h>

namespace FC {

class FCGEOMETRYINTERFACE_API FCAbsGeoPart : public FCAbsGeoCommand, public FCGeoCommandManager
{
    FC_CLASS(FC, FCAbsGeoPart);
public:
    FCAbsGeoPart() = default;
    ~FCAbsGeoPart() override = default;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    virtual bool backtrackingShape();
    void setAutoMergeSolid(bool flag);
    bool getAutoMergeSolid();
    bool update() override;

    void removeDataObj(FCAbstractDataObject* obj) override;
    void removeDataObjWithoutRelease(FCAbstractDataObject* obj) override;
    void removeDataByIndex(int index) override;
    void removeDataByID(int id) override;
    void removeDataByName(const QString& name, bool compSens = false) override;

protected:
    bool _autoMergeSolid{ true };
};

} // namespace FC

#endif // FCABSGEOPART_H
