/**
 * @file FCGeoModelOperSolid.cpp
 * @brief 体特征处理命令实现（接口层）
 */
#include "FCGeoModelOperSolid.h"

namespace FC {

FCGeoEnum::FITKGeometryComType FCGeoModelChamferSolid::getGeometryCommandType()
{
    return FCGeoEnum::FGTChamferSolid;
}
bool FCGeoModelChamferSolid::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelFilletSolid::getGeometryCommandType()
{
    return FCGeoEnum::FGTFilletSolid;
}
bool FCGeoModelFilletSolid::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelDefeatureSolid::getGeometryCommandType()
{
    return FCGeoEnum::FGTDefeature;
}
bool FCGeoModelDefeatureSolid::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelRemoveChamferSolid::getGeometryCommandType()
{
    return FCGeoEnum::FGTRemoveChamfer;
}
bool FCGeoModelRemoveChamferSolid::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelRemoveFilletSolid::getGeometryCommandType()
{
    return FCGeoEnum::FGTRemoveFillet;
}
bool FCGeoModelRemoveFilletSolid::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelFillHoleSolid::getGeometryCommandType()
{
    return FCGeoEnum::FGTFillHole;
}
bool FCGeoModelFillHoleSolid::update() { return true; }

} // namespace FC
