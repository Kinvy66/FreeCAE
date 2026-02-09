/**
 * @file FCGeoTransformation.cpp
 * @brief 几何变换命令实现（接口层）
 */
#include "FCAbsGeoTransformation.h"

namespace FC {

FCGeoEnum::FCGeometryComType FCGeoModelTransform::getGeometryCommandType()
{
    return FCGeoEnum::FGTTransform;
}
bool FCGeoModelTransform::update() { return true; }

FCGeoEnum::FCGeometryComType FCGeoModelTransformByTwoPoints::getGeometryCommandType()
{
    return FCGeoEnum::FGTTransformByTwoPoints;
}
bool FCGeoModelTransformByTwoPoints::update() { return true; }

FCGeoEnum::FCGeometryComType FCGeoModelTransformByDirAndDis::getGeometryCommandType()
{
    return FCGeoEnum::FGTTransformByDirAndDis;
}
bool FCGeoModelTransformByDirAndDis::update() { return true; }

FCGeoEnum::FCGeometryComType FCGeoModelRotate::getGeometryCommandType()
{
    return FCGeoEnum::FGTRotate;
}
bool FCGeoModelRotate::update() { return true; }

FCGeoEnum::FCGeometryComType FCGeoModelScale::getGeometryCommandType()
{
    return FCGeoEnum::FGTScale;
}
bool FCGeoModelScale::update() { return true; }

FCGeoEnum::FCGeometryComType FCGeoModelMirror::getGeometryCommandType()
{
    return FCGeoEnum::FGTMirror;
}
bool FCGeoModelMirror::update() { return true; }

FCGeoEnum::FCGeometryComType FCGeoModelPattern::getGeometryCommandType()
{
    return FCGeoEnum::FGTPattern;
}
bool FCGeoModelPattern::update() { return true; }

FCGeoEnum::FCGeometryComType FCGeoModelRectangularPattern::getGeometryCommandType()
{
    return FCGeoEnum::FGTRectangularPattern;
}
bool FCGeoModelRectangularPattern::update() { return true; }

FCGeoEnum::FCGeometryComType FCGeoModelCircularPattern::getGeometryCommandType()
{
    return FCGeoEnum::FGTCircularPattern;
}
bool FCGeoModelCircularPattern::update() { return true; }

} // namespace FC
