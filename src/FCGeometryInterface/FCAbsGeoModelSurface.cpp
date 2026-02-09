/**
 * @file FCGeoModelSurface.cpp
 * @brief 曲面几何命令实现（接口层）
 */
#include "FCAbsGeoModelSurface.h"

namespace FC {

FCGeoEnum::FITKGeometryComType FCGeoModelClosedWireSurface::getGeometryCommandType()
{
    return FCGeoEnum::FGTClosedWireSurface;
}
bool FCGeoModelClosedWireSurface::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelOffsetSurface::getGeometryCommandType()
{
    return FCGeoEnum::FGTOffsetSurface;
}
bool FCGeoModelOffsetSurface::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelExtrudeSurface::getGeometryCommandType()
{
    return FCGeoEnum::FGTExtrudeSurface;
}
bool FCGeoModelExtrudeSurface::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelRevolSurface::getGeometryCommandType()
{
    return FCGeoEnum::FGTRevolSurface;
}
bool FCGeoModelRevolSurface::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelSweepSurface::getGeometryCommandType()
{
    return FCGeoEnum::FGTSweepSurface;
}
bool FCGeoModelSweepSurface::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelMultiSectionSurface::getGeometryCommandType()
{
    return FCGeoEnum::FGTMultiSectionSurface;
}
bool FCGeoModelMultiSectionSurface::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelBridgeSurface::getGeometryCommandType()
{
    return FCGeoEnum::FGTBridgeSurface;
}
bool FCGeoModelBridgeSurface::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelSolidSurface::getGeometryCommandType()
{
    return FCGeoEnum::FGTSurfaceFromSolid;
}
bool FCGeoModelSolidSurface::update() { return true; }

} // namespace FC
