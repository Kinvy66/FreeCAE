/**
 * @file FCModelEnum.cpp
 * @brief 模型枚举实现（移植自 FITKInterfaceModel）
 */
#include "FCModelEnum.h"

namespace FC {

FCModelEnum::FITKMeshDim FCModelEnum::GetElementDim(FCModelEnum::FITKEleType eletype)
{
    switch (eletype)
    {
    case FCModelEnum::Line2:
    case FCModelEnum::Line3:
        return FCModelEnum::FMDimD1;
    case FCModelEnum::Tri3:
    case FCModelEnum::Tri6:
    case FCModelEnum::Quad4:
    case FCModelEnum::Quad8:
        return FCModelEnum::FMDimD2;
    case FCModelEnum::Tet4:
    case FCModelEnum::Tet10:
    case FCModelEnum::Wedge6:
    case FCModelEnum::Hex8:
    case FCModelEnum::Hex20:
        return FCModelEnum::FMDimD3;
    default:
        break;
    }
    return FCModelEnum::FMDimNone;
}

bool FCModelEnum::HasMeshDimFlag(const unsigned int flags, const FITKMeshDim& dim)
{
    return (flags & dim) != 0;
}

} // namespace FC
