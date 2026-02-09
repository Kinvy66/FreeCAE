/**
 * @file FCGeoModelSurface.cpp
 * @brief 曲面几何命令实现（接口层）
 */
#include "FCAbsGeoModelSurface.h"

namespace FC {

FCGeoEnum::FCGeometryComType FCGeoModelClosedWireSurface::getGeometryCommandType()
{
    return FCGeoEnum::FGTClosedWireSurface;
}
bool FCGeoModelClosedWireSurface::update() { return true; }

FCGeoEnum::FCGeometryComType FCGeoModelOffsetSurface::getGeometryCommandType()
{
    return FCGeoEnum::FGTOffsetSurface;
}
bool FCGeoModelOffsetSurface::update() { return true; }

FCGeoEnum::FCGeometryComType FCGeoModelExtrudeSurface::getGeometryCommandType()
{
    return FCGeoEnum::FGTExtrudeSurface;
}
bool FCGeoModelExtrudeSurface::update() { return true; }

FCGeoEnum::FCGeometryComType FCGeoModelRevolSurface::getGeometryCommandType()
{
    return FCGeoEnum::FGTRevolSurface;
}
bool FCGeoModelRevolSurface::update() { return true; }

FCGeoEnum::FCGeometryComType FCGeoModelSweepSurface::getGeometryCommandType()
{
    return FCGeoEnum::FGTSweepSurface;
}
bool FCGeoModelSweepSurface::update() { return true; }

FCGeoEnum::FCGeometryComType FCGeoModelMultiSectionSurface::getGeometryCommandType()
{
    return FCGeoEnum::FGTMultiSectionSurface;
}
bool FCGeoModelMultiSectionSurface::update() { return true; }

FCGeoEnum::FCGeometryComType FCGeoModelBridgeSurface::getGeometryCommandType()
{
    return FCGeoEnum::FGTBridgeSurface;
}
bool FCGeoModelBridgeSurface::update() { return true; }

FCGeoEnum::FCGeometryComType FCGeoModelSolidSurface::getGeometryCommandType()
{
    return FCGeoEnum::FGTSurfaceFromSolid;
}
bool FCGeoModelSolidSurface::update() { return true; }

} // namespace FC
