/**
 * @file FCAbsGeoInstance.cpp
 * @brief 装配实例命令实现（移植自 FITKInterfaceGeometry）
 */
#include "FCAbsGeoInstance.h"
#include "FCAbsGeoPart.h"
#include <FCData/FCDataRepo.h>

namespace FC {

FCGeoEnum::FCGeometryComType FCAbsGeoInstance::getGeometryCommandType()
{
    return FCGeoEnum::FGTInstance;
}

bool FCAbsGeoInstance::update()
{
    return true;
}

void FCAbsGeoInstance::setPartCommandID(int id)
{
    _partCmdId = id;
}

int FCAbsGeoInstance::getPartCommandID()
{
    return _partCmdId;
}

FCAbsGeoPart* FCAbsGeoInstance::getPartCommand()
{
    return FCDataRepo::instance()->getDataAs<FCAbsGeoPart>(_partCmdId);
}

} // namespace FC
