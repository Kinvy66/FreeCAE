/**
 * @file FCGeoModelPartitionFace.cpp
 * @brief 面分块命令实现（接口层）
 */
#include "FCGeoModelPartitionFace.h"

namespace FC {

FCGeoEnum::FITKGeometryComType FCGeoModelPartitionFaceWithSketch::getGeometryCommandType()
{
    return FCGeoEnum::FGTPartitionFaceWithSketch;
}
bool FCGeoModelPartitionFaceWithSketch::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelPartitionFaceWithTwoPoints::getGeometryCommandType()
{
    return FCGeoEnum::FGTPartitionFaceWithTwoPoints;
}
bool FCGeoModelPartitionFaceWithTwoPoints::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelPartitionFaceWithDatumPlane::getGeometryCommandType()
{
    return FCGeoEnum::FGTPartitionFaceWithDatumPlane;
}
bool FCGeoModelPartitionFaceWithDatumPlane::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelPartitionFaceWithExtendFace::getGeometryCommandType()
{
    return FCGeoEnum::FGTPartitionFaceWithExtendFace;
}
bool FCGeoModelPartitionFaceWithExtendFace::update() { return true; }

} // namespace FC
