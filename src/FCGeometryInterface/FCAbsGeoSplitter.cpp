/**
 * @file FCGeoSplitter.cpp
 * @brief 模型分割命令实现（接口层�?
 */
#include "FCAbsGeoSplitter.h"

namespace FC {

FCGeoEnum::FITKGeometryComType FCGeoCurveSplitter::getGeometryCommandType()
{
    return FCGeoEnum::FGTCurveSplitter;
}

bool FCGeoCurveSplitter::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoSurfaceSplitter::getGeometryCommandType()
{
    return FCGeoEnum::FGTSurfaceSplitter;
}

bool FCGeoSurfaceSplitter::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoSolidSplitter::getGeometryCommandType()
{
    return FCGeoEnum::FGTSolidSplitter;
}

bool FCGeoSolidSplitter::update() { return true; }

} // namespace FC
