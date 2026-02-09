/**
 * @file FCGeoSplitter.cpp
 * @brief 模型分割命令实现（接口层）
 */
#include "FCAbsGeoSplitter.h"

namespace FC {

FCGeoEnum::FCGeometryComType FCGeoCurveSplitter::getGeometryCommandType()
{
    return FCGeoEnum::FGTCurveSplitter;
}

bool FCGeoCurveSplitter::update() { return true; }

FCGeoEnum::FCGeometryComType FCGeoSurfaceSplitter::getGeometryCommandType()
{
    return FCGeoEnum::FGTSurfaceSplitter;
}

bool FCGeoSurfaceSplitter::update() { return true; }

FCGeoEnum::FCGeometryComType FCGeoSolidSplitter::getGeometryCommandType()
{
    return FCGeoEnum::FGTSolidSplitter;
}

bool FCGeoSolidSplitter::update() { return true; }

} // namespace FC
