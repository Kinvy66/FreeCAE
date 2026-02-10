/**
 * @file FCVTKGraphObjectModelShape.h
 * @brief 几何形状代理 -> VTK 图元（移植自 FITKFluidVTKGraphObjectModelShape，直接绑定 FCAbsGeoShapeAgent）
 */
#ifndef FCVTKGRAPHOBJECTMODELSHAPE_H
#define FCVTKGRAPHOBJECTMODELSHAPE_H

#include "FCVTKGraphAdaptorAPI.h"
#include "FCVTKGraphObjectModelBase.h"

namespace FC {

class FCAbsGeoShapeAgent;

class FCVTKGRAPHADAPTOR_API FCVTKGraphObjectModelShape : public FCVTKGraphObjectModelBase
{
public:
    explicit FCVTKGraphObjectModelShape(FCAbsGeoShapeAgent* modelData);
    ~FCVTKGraphObjectModelShape() override = default;
    void update(bool forceUpdate = false) override;
};

} // namespace FC

#endif // FCVTKGRAPHOBJECTMODELSHAPE_H
