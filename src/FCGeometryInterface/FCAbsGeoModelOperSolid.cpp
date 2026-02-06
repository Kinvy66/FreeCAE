/**
 * @file FCGeoModelOperSolid.cpp
 * @brief 体特征处理命令实现（接口层）
 */
#include "FCAbsGeoModelOperSolid.h"

namespace FC {

FCGeoEnum::FITKGeometryComType FCAbsGeoModelChamferSolid::getGeometryCommandType()
{
    return FCGeoEnum::FGTChamferSolid;
}
bool FCAbsGeoModelChamferSolid::update() { return true; }

FCGeoEnum::FITKGeometryComType FCAbsGeoModelFilletSolid::getGeometryCommandType()
{
    return FCGeoEnum::FGTFilletSolid;
}
bool FCAbsGeoModelFilletSolid::update() { return true; }

FCGeoEnum::FITKGeometryComType FCAbsGeoModelDefeatureSolid::getGeometryCommandType()
{
    return FCGeoEnum::FGTDefeature;
}
bool FCAbsGeoModelDefeatureSolid::update() { return true; }

FCGeoEnum::FITKGeometryComType FCAbsGeoModelRemoveChamferSolid::getGeometryCommandType()
{
    return FCGeoEnum::FGTRemoveChamfer;
}
bool FCAbsGeoModelRemoveChamferSolid::update() { return true; }

FCGeoEnum::FITKGeometryComType FCAbsGeoModelRemoveFilletSolid::getGeometryCommandType()
{
    return FCGeoEnum::FGTRemoveFillet;
}
bool FCAbsGeoModelRemoveFilletSolid::update() { return true; }

FCGeoEnum::FITKGeometryComType FCAbsGeoModelFillHoleSolid::getGeometryCommandType()
{
    return FCGeoEnum::FGTFillHole;
}
bool FCAbsGeoModelFillHoleSolid::update() { return true; }

FCGeoEnum::FITKGeometryComType FCAbsGeoModelCircularHoleSolid::getGeometryCommandType()
{
    return FCGeoEnum::FGTCircularHole;
}
bool FCAbsGeoModelCircularHoleSolid::update() { return true; }

void FCAbsGeoModelCircularHoleSolid::setHolePoint(double x, double y, double z)
{
    _holePoint[0] = x; _holePoint[1] = y; _holePoint[2] = z;
}

void FCAbsGeoModelCircularHoleSolid::getHolePoint(double& x, double& y, double& z) const
{
    x = _holePoint[0]; y = _holePoint[1]; z = _holePoint[2];
}

void FCAbsGeoModelCircularHoleSolid::setHolePoint(const double* point)
{
    if (!point) return;
    _holePoint[0] = point[0]; _holePoint[1] = point[1]; _holePoint[2] = point[2];
}

void FCAbsGeoModelCircularHoleSolid::getHolePoint(double* point) const
{
    if (!point) return;
    point[0] = _holePoint[0]; point[1] = _holePoint[1]; point[2] = _holePoint[2];
}

} // namespace FC
