/**
 * @file FCGeoSampleCurve.cpp
 * @brief 曲线采样命令实现（接口层）
 */
#include "FCGeoSampleCurve.h"

namespace FC {

FCGeoEnum::FITKGeometryComType FCGeoSampleCurve::getGeometryCommandType()
{
    return FCGeoEnum::FGTSampleCurve;
}

bool FCGeoSampleCurve::update()
{
    return true;
}

} // namespace FC
