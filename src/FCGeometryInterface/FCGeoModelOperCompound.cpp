/**
 * @file FCGeoModelOperCompound.cpp
 * @brief 复合体操作命令实现（接口层）
 */
#include "FCGeoModelOperCompound.h"

namespace FC {

FCGeoEnum::FITKGeometryComType FCGeoModelOperCompound::getGeometryCommandType()
{
    return FCGeoEnum::FGTCompoundAppendShape;
}

bool FCGeoModelOperCompound::update()
{
    return true;
}

} // namespace FC
