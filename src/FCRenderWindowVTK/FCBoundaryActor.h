/**
 * @file FCBoundaryActor.h
 * @brief 边界范围 actor
 */
#ifndef FCBOUNDARYACTOR_H
#define FCBOUNDARYACTOR_H

#include "FCRenderWindowVTKAPI.h"

class vtkActor;
class vtkUnstructuredGrid;

namespace FC
{
class FCRENDERWINDOWVTK_API FCBoundaryActor
{
public:
    explicit FCBoundaryActor();
    virtual ~FCBoundaryActor();

    vtkActor* getActor();
    void updateBoundary(double* bound);

private:
    vtkActor* mBoundaryActor = nullptr;
};
} // namespace FC
#endif
