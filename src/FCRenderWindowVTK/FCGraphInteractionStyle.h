/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCGraphInteractionStyle.h
 * @brief 渲染窗口交互器类接口声明（仅 VTK+Qt，与 FITKGraphInteractionStyle 接口一致）
 */
#ifndef FCGRAPHINTERACTIONSTYLE_H
#define FCGRAPHINTERACTIONSTYLE_H

#include "FCRenderWindowVTKAPI.h"
#include <QObject>
#include <vtkInteractorStyleRubberBandPick.h>

class vtkRenderer;
class vtkRenderWindowInteractor;

namespace Comp
{
class FCGraphOperator;
class FCGraph3DWindowVTK;
class FCGraphAreaPicker;

class FCRENDERWINDOWVTK_API FCGraphInteractionStyle : public QObject, public vtkInteractorStyleRubberBandPick
{
    Q_OBJECT
public:
    static FCGraphInteractionStyle* New();
    vtkTypeMacro(FCGraphInteractionStyle, vtkInteractorStyleRubberBandPick);

    void setGraph3DWin(FCGraph3DWindowVTK* graphWin);
    void setInteractor(vtkRenderWindowInteractor* interactor);
    void setAreaPickRenderer(vtkRenderer* renderer);
    vtkRenderWindowInteractor* getInteractor();
    void setAredPickerState(bool state);
    virtual void areaPick(int* startPos, int* endPos);
    virtual void applyDolly(double factor);

protected:
    FCGraphInteractionStyle();
    ~FCGraphInteractionStyle() override;

    void OnLeftButtonDown() override;
    void OnLeftButtonUp() override;
    void OnMiddleButtonDown() override;
    void OnMiddleButtonUp() override;
    void OnMouseMove() override;
    void OnMouseWheelForward() override;
    void OnMouseWheelBackward() override;
    void OnRightButtonDown() override;
    void OnRightButtonUp() override;
    void OnChar() override;
    bool isMouseMoved();

    FCGraph3DWindowVTK* m_graph3dWin = nullptr;
    FCGraphOperator* m_operactor = nullptr;
    int m_leftButtonDowmPos[2] = { 0, 0 };
    int m_leftButtonUpPos[2] = { 0, 0 };
    FCGraphAreaPicker* m_areaPick = nullptr;
    bool m_leftButtonPressed = false;
};
} // namespace Comp
#endif
