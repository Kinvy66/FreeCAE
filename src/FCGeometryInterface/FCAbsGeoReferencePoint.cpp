/**
 * @file FCGeoReferencePoint.cpp
  * @brief 参考点命令实现（接口层）
 */
#include "FCAbsGeoReferencePoint.h"

namespace FC {

FCGeoEnum::FITKDatumType FCGeoReferencePoint::getDatumType()
{
    return FCGeoEnum::FDTPoint;
}

void FCGeoReferencePoint::setCoord(double x, double y, double z)
{
    _pos[0] = x; _pos[1] = y; _pos[2] = z;
}

FCGeoEnum::FITKDatumType FCGeoReferenceCurveRadio::getDatumType()
{
    return FCGeoEnum::FDTCurveRadio;
}

} // namespace FC
