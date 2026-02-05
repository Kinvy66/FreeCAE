/**
 * @file FCGeoModelExport.cpp
 * @brief 几何导出命令实现（接口层）
 */
#include "FCGeoModelExport.h"

namespace FC {

FCGeoEnum::FITKGeometryComType FCGeoModelExport::getGeometryCommandType()
{
    return FCGeoEnum::FGTExport;
}

bool FCGeoModelExport::update()
{
    return true;
}

void FCGeoModelExport::setFileName(const QString& fileName)
{
    _exportFileName = fileName;
}

QString FCGeoModelExport::getFileName() const
{
    return _exportFileName;
}

void FCGeoModelExport::addExportCommandID(int id)
{
    if (!_exportIds.contains(id))
        _exportIds.append(id);
}

void FCGeoModelExport::removeExportCommandID(int id)
{
    _exportIds.removeAll(id);
}

} // namespace FC
