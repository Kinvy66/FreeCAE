/**
 * @file FCGeoModelPartitionSolid.cpp
 * @brief 实体分块命令实现（接口层）
 */
#include "FCGeoModelPartitionSolid.h"

namespace FC {

FCGeoEnum::FITKGeometryComType FCGeoModelPartitionSolidWithPlane::getGeometryCommandType()
{
    return FCGeoEnum::FGTPartitionSolidWithPlane;
}
bool FCGeoModelPartitionSolidWithPlane::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelPartitionSolidWithExtendFace::getGeometryCommandType()
{
    return FCGeoEnum::FGTPartitionSolidWithExtendFace;
}
bool FCGeoModelPartitionSolidWithExtendFace::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelPartitionSolidWithSketchPlanar::getGeometryCommandType()
{
    return FCGeoEnum::FGTPartitionSolidWithSketchPlanar;
}
bool FCGeoModelPartitionSolidWithSketchPlanar::update() { return true; }

} // namespace FC
