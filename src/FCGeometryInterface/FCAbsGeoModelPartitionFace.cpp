/**
 * @file FCGeoModelPartitionFace.cpp
 * @brief 面分块命令实现（接口层）
 */
#include "FCAbsGeoModelPartitionFace.h"

namespace FC {

FCGeoEnum::FCGeometryComType FCGeoModelPartitionFaceWithSketch::getGeometryCommandType()
{
    return FCGeoEnum::FGTPartitionFaceWithSketch;
}
bool FCGeoModelPartitionFaceWithSketch::update() { return true; }

FCGeoEnum::FCGeometryComType FCGeoModelPartitionFaceWithTwoPoints::getGeometryCommandType()
{
    return FCGeoEnum::FGTPartitionFaceWithTwoPoints;
}
bool FCGeoModelPartitionFaceWithTwoPoints::update() { return true; }

FCGeoEnum::FCGeometryComType FCGeoModelPartitionFaceWithDatumPlane::getGeometryCommandType()
{
    return FCGeoEnum::FGTPartitionFaceWithDatumPlane;
}
bool FCGeoModelPartitionFaceWithDatumPlane::update() { return true; }

FCGeoEnum::FCGeometryComType FCGeoModelPartitionFaceWithExtendFace::getGeometryCommandType()
{
    return FCGeoEnum::FGTPartitionFaceWithExtendFace;
}
bool FCGeoModelPartitionFaceWithExtendFace::update() { return true; }

FCGeoEnum::FCGeometryComType FCGeoModelPartitionFaceWithCurvedPath::getGeometryCommandType()
{
    return FCGeoEnum::FGTPartitionFaceWithCurvedPath;
}
bool FCGeoModelPartitionFaceWithCurvedPath::update() { return true; }

FCGeoEnum::FCGeometryComType FCGeoModelPartitionFaceWithIntersectFace::getGeometryCommandType()
{
    return FCGeoEnum::FGTPartitionFaceWithIntersectFace;
}
bool FCGeoModelPartitionFaceWithIntersectFace::update() { return true; }

FCGeoEnum::FCGeometryComType FCGeoModelPartitionFaceWithProjectEdges::getGeometryCommandType()
{
    return FCGeoEnum::FGTPartitionFaceWithProjectEdges;
}
bool FCGeoModelPartitionFaceWithProjectEdges::update() { return true; }

} // namespace FC
