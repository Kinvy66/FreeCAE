/**
 * @file FCGeoModelPartitionEdge.cpp
 * @brief 边分块命令实现（接口层）
 */
#include "FCAbsGeoModelPartitionEdge.h"

namespace FC {

FCGeoEnum::FITKGeometryComType FCGeoModelPartitionEdgeWithParameter::getGeometryCommandType()
{
    return FCGeoEnum::FGTPartitionEdgeWithParameter;
}
bool FCGeoModelPartitionEdgeWithParameter::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelPartitionEdgeWithPoint::getGeometryCommandType()
{
    return FCGeoEnum::FGTPartitionEdgeWithPoint;
}
bool FCGeoModelPartitionEdgeWithPoint::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelPartitionEdgeWithDatumPlane::getGeometryCommandType()
{
    return FCGeoEnum::FGTPartitionEdgeWithDatumPlane;
}
bool FCGeoModelPartitionEdgeWithDatumPlane::update() { return true; }

} // namespace FC
