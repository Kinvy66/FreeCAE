/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 */

#include "FCBoundaryActor.h"
#include <vtkActor.h>
#include <vtkDataSetMapper.h>
#include <vtkPoints.h>
#include <vtkIdList.h>
#include <vtkSmartPointer.h>
#include <vtkCellType.h>
#include <vtkUnstructuredGrid.h>
#include <vtkProperty.h>

namespace Comp
{
FCBoundaryActor::FCBoundaryActor()
{
    _boundaryActor = vtkActor::New();
    _boundaryActor->SetMapper(vtkSmartPointer<vtkDataSetMapper>::New());
    vtkSmartPointer<vtkUnstructuredGrid> ugrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(0., 0., 0.);
    points->InsertNextPoint(0., 0., 0.);
    vtkSmartPointer<vtkIdList> c1 = vtkSmartPointer<vtkIdList>::New();
    c1->InsertNextId(0);
    vtkSmartPointer<vtkIdList> c2 = vtkSmartPointer<vtkIdList>::New();
    c2->InsertNextId(1);
    ugrid->InsertNextCell(VTK_VERTEX, c1);
    ugrid->InsertNextCell(VTK_VERTEX, c2);
    ugrid->SetPoints(points);
    _boundaryActor->GetMapper()->SetInputDataObject(ugrid);
    _boundaryActor->GetProperty()->SetOpacity(0);
}

FCBoundaryActor::~FCBoundaryActor()
{
    if (_boundaryActor)
        _boundaryActor->Delete();
}

vtkActor* FCBoundaryActor::getActor()
{
    return _boundaryActor;
}

void FCBoundaryActor::updateBoundary(double* bound)
{
    vtkUnstructuredGrid* ugrid =
        vtkUnstructuredGrid::SafeDownCast(_boundaryActor->GetMapper()->GetInput());
    if (!ugrid)
        return;
    vtkPoints* pts = ugrid->GetPoints();
    pts->Reset();
    pts->InsertNextPoint(bound[0], bound[2], bound[4]);
    pts->InsertNextPoint(bound[1], bound[3], bound[5]);
    ugrid->Modified();
}
} // namespace Comp
