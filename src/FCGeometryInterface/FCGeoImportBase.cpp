/**
 * @file FCGeoImportBase.cpp
 * @brief 几何导入命令抽象基类实现
 */
#include "FCGeoImportBase.h"

namespace FC {

void FCGeoImportBase::setFileName(const QString& fileName)
{
    _importFileName = fileName;
}

QString FCGeoImportBase::getFileName() const
{
    return _importFileName;
}

} // namespace FC
