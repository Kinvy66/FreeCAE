/**
 * @file FCGeoMultiDelete.cpp
 * @brief 多选模型删除命令实现（undo/redo 需与命令管理器/Repo 集成后实现）
 */
#include "FCAbsGeoMultiDelete.h"

namespace FC {

FCGeoEnum::FITKGeometryComType FCGeoMultiDelete::getGeometryCommandType()
{
    return FCGeoEnum::FGTMultiDelete;
}

bool FCGeoMultiDelete::update()
{
    return redo();
}

bool FCGeoMultiDelete::undo()
{
    return true;
}

bool FCGeoMultiDelete::redo()
{
    return true;
}

void FCGeoMultiDelete::setDeleteCommandIDs(const QList<int>& ids)
{
    m_deletedCmdIds = ids;
}

QList<int> FCGeoMultiDelete::getDeleteCommandIDs() const
{
    return m_deletedCmdIds;
}

} // namespace FC
