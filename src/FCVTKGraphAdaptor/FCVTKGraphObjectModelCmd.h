/**
 * @file FCVTKGraphObjectModelCmd.h
 * @brief 几何命令 -> VTK 图元（移植自 FITKFluidVTKGraphAdaptor FITKFluidVTKGraphObjectModelCmd）
 */
#ifndef FCVTKGRAPHOBJECTMODELCMD_H
#define FCVTKGRAPHOBJECTMODELCMD_H

#include "FCVTKGraphAdaptorAPI.h"
#include "FCVTKGraphObjectModelBase.h"

namespace FC {

class FCAbsGeoCommand;

class FCVTKGRAPHADAPTOR_API FCVTKGraphObjectModelCmd : public FCVTKGraphObjectModelBase
{
public:
    explicit FCVTKGraphObjectModelCmd(FCAbsGeoCommand* modelData);
    ~FCVTKGraphObjectModelCmd() override = default;
    void update(bool forceUpdate = false) override;
};

} // namespace FC

#endif // FCVTKGRAPHOBJECTMODELCMD_H
