/**
 * @file FCGeoModelExportTopos.cpp
 * @brief 导出几何拓扑命令实现（接口层）
 */
#include "FCAbsGeoModelExportTopos.h"

namespace FC {

FCGeoEnum::FCGeometryComType FCGeoModelExportTopos::getGeometryCommandType()
{
    return FCGeoEnum::FGTExportTopos;
}

bool FCGeoModelExportTopos::update()
{
    return true;
}

void FCGeoModelExportTopos::setFileName(const QString& fileName)
{
    _exportFileName = fileName;
}

QString FCGeoModelExportTopos::getFileName() const
{
    return _exportFileName;
}

} // namespace FC
