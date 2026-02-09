/**
 * @file FCGeoDelete.h
  * @brief 删除几何命令（移植自 FITKInterfaceGeometry FITKGeoDelete）
 */
#ifndef FCGEODELETE_H
#define FCGEODELETE_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"

namespace FC {

/**
 * @brief 模型删除命令：将指定命令置为删除状�?
 */
class FCGEOMETRYINTERFACE_API FCGeoDelete : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoDelete);
public:
    FCGeoDelete() = default;
    ~FCGeoDelete() override = default;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
    bool undo() override;
    bool redo() override;

    void setDeleteCommandID(int id);
    int getDeleteCommandID() const;

protected:
    int m_deletedCmdId = -1;
};

} // namespace FC

#endif // FCGEODELETE_H
