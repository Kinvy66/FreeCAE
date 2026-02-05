/**
 * @file FCGeoOperBool.cpp
 * @brief 布尔运算几何命令实现（接口层）
 */
#include "FCGeoOperBool.h"
#include "FCAbsGeoShapeAgent.h"

namespace FC {

FCGeoEnum::FITKGeometryComType FCGeoOperBool::getGeometryCommandType()
{
    return FCGeoEnum::FGTBool;
}

bool FCGeoOperBool::update()
{
    return true;
}

void FCGeoOperBool::setBoolOperType(GeoBoolOperType t)
{
    _boolOperType = t;
}

FCGeoOperBool::GeoBoolOperType FCGeoOperBool::getBoolOperType() const
{
    return _boolOperType;
}

void FCGeoOperBool::setGeoShape1(FCAbsGeoShapeAgent* s1)
{
    _shape1 = s1;
}

void FCGeoOperBool::setGeoShape2(FCAbsGeoShapeAgent* s2)
{
    _shape2 = s2;
}

FCAbsGeoShapeAgent* FCGeoOperBool::getGeoShape1()
{
    return _shape1;
}

FCAbsGeoShapeAgent* FCGeoOperBool::getGeoShape2()
{
    return _shape2;
}

} // namespace FC
