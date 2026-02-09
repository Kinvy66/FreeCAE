/**
 * @file FCGeoModelPartitionSolid.cpp
 * @brief 实体分块命令实现（接口层） */
#include "FCAbsGeoModelPartitionSolid.h"

namespace FC {

FCGeoEnum::FCGeometryComType FCGeoModelPartitionSolidWithPlane::getGeometryCommandType()
{
    return FCGeoEnum::FGTPartitionSolidWithPlane;
}
bool FCGeoModelPartitionSolidWithPlane::update() { return true; }

FCGeoEnum::FCGeometryComType FCGeoModelPartitionSolidWithExtendFace::getGeometryCommandType()
{
    return FCGeoEnum::FGTPartitionSolidWithExtendFace;
}
bool FCGeoModelPartitionSolidWithExtendFace::update() { return true; }

FCGeoEnum::FCGeometryComType FCGeoModelPartitionSolidWithSketchPlanar::getGeometryCommandType()
{
    return FCGeoEnum::FGTPartitionSolidWithSketchPlanar;
}
bool FCGeoModelPartitionSolidWithSketchPlanar::update() { return true; }

FCGeoEnum::FCGeometryComType FCGeoModelPartitionSolidWithSweepEdge::getGeometryCommandType()
{
    return FCGeoEnum::FGTPartitionSolidWithSweepEdge;
}
bool FCGeoModelPartitionSolidWithSweepEdge::update() { return true; }

FCGeoEnum::FCGeometryComType FCGeoModelPartitionSolidWithNSidedPatch::getGeometryCommandType()
{
    return FCGeoEnum::FGTPartitionSolidWithNSidedPatch;
}
bool FCGeoModelPartitionSolidWithNSidedPatch::update() { return true; }

} // namespace FC
