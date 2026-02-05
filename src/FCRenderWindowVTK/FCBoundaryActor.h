/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCBoundaryActor.h
 * @brief 边界范围 actor（仅 VTK，与 FITKBoundaryActor 接口一致）
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
    vtkActor* _boundaryActor = nullptr;
};
} // namespace FC
#endif
