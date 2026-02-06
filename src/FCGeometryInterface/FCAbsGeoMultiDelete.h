/**
 * @file FCGeoMultiDelete.h
 * @brief 多选模型删除命令接口（移植�?FITKInterfaceGeometry FITKAbsGeoMultiDelete�?
 */
#ifndef FCGEOMULTIDELETE_H
#define FCGEOMULTIDELETE_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include <QList>

namespace FC {

/**
 * @brief 多选模型删除命�?
 */
class FCGEOMETRYINTERFACE_API FCGeoMultiDelete : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoMultiDelete);
public:
    FCGeoMultiDelete() = default;
    ~FCGeoMultiDelete() override = default;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
    bool undo() override;
    bool redo() override;

    void setDeleteCommandIDs(const QList<int>& ids);
    QList<int> getDeleteCommandIDs() const;

protected:
    QList<int> m_deletedCmdIds;
};

} // namespace FC

#endif // FCGEOMULTIDELETE_H
