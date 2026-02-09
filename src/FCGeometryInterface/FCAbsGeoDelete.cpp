/**
 * @file FCGeoDelete.cpp
  * @brief 删除几何命令实现（接口层）
 */
#include "FCAbsGeoDelete.h"

namespace FC {

FCGeoEnum::FCGeometryComType FCGeoDelete::getGeometryCommandType()
{
    return FCGeoEnum::FGTDelete;
}

bool FCGeoDelete::update()
{
    return redo();
}

bool FCGeoDelete::undo()
{
    // 需通过 FCGeoCommandManager/Repo 获取命令并设置命令状态为 FGSNormal
    return true;
}

bool FCGeoDelete::redo()
{
    // 需通过 FCGeoCommandManager/Repo 获取命令并设置命令状态为 FGSDeleted
    return true;
}

void FCGeoDelete::setDeleteCommandID(int id)
{
    m_deletedCmdId = id;
}

int FCGeoDelete::getDeleteCommandID() const
{
    return m_deletedCmdId;
}

} // namespace FC
