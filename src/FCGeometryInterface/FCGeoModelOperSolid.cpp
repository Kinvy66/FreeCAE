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

FCGeoEnum::FITKGeometryComType FCGeoModelCircularHoleSolid::getGeometryCommandType()
{
    return FCGeoEnum::FGTCircularHole;
}
bool FCGeoModelCircularHoleSolid::update() { return true; }

void FCGeoModelCircularHoleSolid::setHolePoint(double x, double y, double z)
{
    _holePoint[0] = x; _holePoint[1] = y; _holePoint[2] = z;
}

void FCGeoModelCircularHoleSolid::getHolePoint(double& x, double& y, double& z) const
{
    x = _holePoint[0]; y = _holePoint[1]; z = _holePoint[2];
}

void FCGeoModelCircularHoleSolid::setHolePoint(const double* point)
{
    if (!point) return;
    _holePoint[0] = point[0]; _holePoint[1] = point[1]; _holePoint[2] = point[2];
}

void FCGeoModelCircularHoleSolid::getHolePoint(double* point) const
{
    if (!point) return;
    point[0] = _holePoint[0]; point[1] = _holePoint[1]; point[2] = _holePoint[2];
}

} // namespace FC
