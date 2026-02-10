/**
 * @file FCVTKViewAdaptorModelCmd.h
 * @brief 几何命令 -> VTK 图元适配器（移植自 FITKFluidVTKViewAdaptorModelCmd，Legacy：直接接收 FCAbsGeoCommand）
 */
#ifndef FCVTKVIEWADAPTORMODELCMD_H
#define FCVTKVIEWADAPTORMODELCMD_H

#include "FCVTKGraphAdaptorAPI.h"
#include "FCVTKViewAdaptorBase.h"

namespace FC {

class FCVTKGRAPHADAPTOR_API FCVTKViewAdaptorModelCmd : public FCVTKViewAdaptorBase
{
public:
    FCVTKViewAdaptorModelCmd() = default;
    ~FCVTKViewAdaptorModelCmd() override = default;
    bool update() override;
};

} // namespace FC

#endif // FCVTKVIEWADAPTORMODELCMD_H
