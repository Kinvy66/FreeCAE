/**
 * @file FCGeoDelete.cpp
 * @brief 模型删除命令实现（undo/redo 需与命令管理器/Repo 集成后实现）
 */
#include "FCGeoDelete.h"

namespace FC {

FCGeoEnum::FITKGeometryComType FCGeoDelete::getGeometryCommandType()
{
    return FCGeoEnum::FGTDelete;
}

bool FCGeoDelete::update()
{
    return redo();
}

bool FCGeoDelete::undo()
{
    // 需通过 FCGeoCommandManager/Repo 获取命令并 setCommandStatus(FGSNormal) 等
    return true;
}

bool FCGeoDelete::redo()
{
    // 需通过 FCGeoCommandManager/Repo 获取命令并 setCommandStatus(FGSDeleted) 等
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
