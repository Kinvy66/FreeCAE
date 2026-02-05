/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

#ifndef FCLEGENDSCALEACTOR_H
#define FCLEGENDSCALEACTOR_H

#include "FCRenderWindowVTKAPI.h"
#include <vtkProp.h>
#include <vtkTimeStamp.h>

class vtkAxisActor2D;
class vtkTextProperty;
class vtkPolyData;
class vtkPolyDataMapper2D;
class vtkActor2D;
class vtkTextMapper;
class vtkPoints;
class vtkCoordinate;

namespace Comp
{
class FCRENDERWINDOWVTK_API FCLegendScaleActor : public vtkProp
{
public:
    static FCLegendScaleActor* New();
    vtkTypeMacro(FCLegendScaleActor, vtkProp);
    void PrintSelf(ostream& os, vtkIndent indent) override;

    vtkGetObjectMacro(BottomAxis, vtkAxisActor2D);

    void BuildRepresentation(vtkViewport* viewport);
    void GetActors2D(vtkPropCollection*) override;
    void ReleaseGraphicsResources(vtkWindow*) override;
    int RenderOverlay(vtkViewport*) override;
    int RenderOpaqueGeometry(vtkViewport*) override;

protected:
    FCLegendScaleActor();
    ~FCLegendScaleActor() override;

    vtkAxisActor2D* BottomAxis;
    vtkTypeBool BottomAxisVisibility;
    vtkPolyData* Legend;
    vtkPoints* LegendPoints;
    vtkPolyDataMapper2D* LegendMapper;
    vtkActor2D* LegendActor;
    vtkPolyData* LegendBorder;
    vtkPolyDataMapper2D* LegendBorderMapper;
    vtkActor2D* LegendBorderActor;
    vtkTextMapper* LabelMappers[5];
    vtkActor2D* LabelActors[5];
    vtkTextProperty* LegendLabelProperty;
    vtkCoordinate* Coordinate;
    vtkTimeStamp BuildTime;

private:
    FCLegendScaleActor(const FCLegendScaleActor&) = delete;
    void operator=(const FCLegendScaleActor&) = delete;
};
} // namespace Comp
#endif
