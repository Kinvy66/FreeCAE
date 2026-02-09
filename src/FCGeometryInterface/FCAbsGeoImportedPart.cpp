/**
 * @file FCGeoImportedPart.cpp
 * @brief 导入的模型部件命令实现（接口层）
 */
#include "FCAbsGeoImportedPart.h"

namespace FC {

FCGeoEnum::FCGeometryComType FCGeoImportedPart::getGeometryCommandType()
{
    return FCGeoEnum::FGTImportPart;
}

bool FCGeoImportedPart::update()
{
    return true;
}

} // namespace FC
