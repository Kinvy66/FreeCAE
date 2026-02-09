/**
 * @file FCGeoModelOperCompound.cpp
 * @brief 复合体操作命令实现（接口层）
 */
#include "FCAbsGeoModelOperCompound.h"

namespace FC {

FCGeoEnum::FCGeometryComType FCGeoModelOperCompound::getGeometryCommandType()
{
    return FCGeoEnum::FGTCompoundAppendShape;
}

bool FCGeoModelOperCompound::update()
{
    return true;
}

} // namespace FC
