/**
 * @file FCVTKViewAdaptorModelShape.h
 * @brief 几何形状代理 -> VTK 图元适配器（移植自 FITKFluidVTKGraphAdaptor FITKFluidVTKViewAdaptorModelShape）
 */
#ifndef FCVTKVIEWADAPTORMODELSHAPE_H
#define FCVTKVIEWADAPTORMODELSHAPE_H

#include "FCVTKGraphAdaptorAPI.h"
#include "FCVTKViewAdaptorBase.h"

namespace FC {

class FCVTKGRAPHADAPTOR_API FCVTKViewAdaptorModelShape : public FCVTKViewAdaptorBase
{
public:
    FCVTKViewAdaptorModelShape() = default;
    ~FCVTKViewAdaptorModelShape() override = default;
    bool update() override;
};

} // namespace FC

#endif // FCVTKVIEWADAPTORMODELSHAPE_H
