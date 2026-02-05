/**
 * @file FCGeoTransformation.cpp
 * @brief 几何变换命令实现（接口层）
 */
#include "FCGeoTransformation.h"

namespace FC {

FCGeoEnum::FITKGeometryComType FCGeoModelTransform::getGeometryCommandType()
{
    return FCGeoEnum::FGTTransform;
}
bool FCGeoModelTransform::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelTransformByTwoPoints::getGeometryCommandType()
{
    return FCGeoEnum::FGTTransformByTwoPoints;
}
bool FCGeoModelTransformByTwoPoints::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelTransformByDirAndDis::getGeometryCommandType()
{
    return FCGeoEnum::FGTTransformByDirAndDis;
}
bool FCGeoModelTransformByDirAndDis::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelRotate::getGeometryCommandType()
{
    return FCGeoEnum::FGTRotate;
}
bool FCGeoModelRotate::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelScale::getGeometryCommandType()
{
    return FCGeoEnum::FGTScale;
}
bool FCGeoModelScale::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelMirror::getGeometryCommandType()
{
    return FCGeoEnum::FGTMirror;
}
bool FCGeoModelMirror::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelPattern::getGeometryCommandType()
{
    return FCGeoEnum::FGTPattern;
}
bool FCGeoModelPattern::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelRectangularPattern::getGeometryCommandType()
{
    return FCGeoEnum::FGTRectangularPattern;
}
bool FCGeoModelRectangularPattern::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelCircularPattern::getGeometryCommandType()
{
    return FCGeoEnum::FGTCircularPattern;
}
bool FCGeoModelCircularPattern::update() { return true; }

} // namespace FC
