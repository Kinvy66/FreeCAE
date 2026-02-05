/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCGraphAreaPicker.h
 * @brief VTK 框选功能接口声明（仅 VTK+Qt，与 FITKGraphAreaPicker 接口一致）
 */
#ifndef FCGRAPHAREAPICKER_H
#define FCGRAPHAREAPICKER_H

#include "FCRenderWindowVTKAPI.h"
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkActor2D.h>

class vtkRenderer;

namespace Comp
{
class FCGraphInteractionStyle;

class FCRENDERWINDOWVTK_API FCGraphAreaPicker
{
public:
    explicit FCGraphAreaPicker(FCGraphInteractionStyle* style);
    ~FCGraphAreaPicker();

    void setLeftButtonDownPos(int* pos);
    void setPickerRender(vtkRenderer* render);
    void enable(bool state = true);
    bool isEnable();
    void drawRectangle();
    void pick();
    void setColor(double rf, double gf, double bf);

private:
    void initRectangle();
    void updateRectangle();

    FCGraphInteractionStyle* m_style = nullptr;
    vtkRenderer* m_render = nullptr;
    int m_startPos[2] = { 0, 0 };
    int m_endPos[2] = { 0, 0 };
    vtkSmartPointer<vtkPolyData> m_pickData;
    vtkSmartPointer<vtkActor2D> m_pickActor;
    double m_color3[3] = { 1., 1., 1. };
};
} // namespace Comp
#endif
