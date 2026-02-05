/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 */

#include "FCGraphInteractionStyle.h"
#include "FCGraphAreaPicker.h"
#include "FCGraph3DWindowVTK.h"
#include <vtkRenderWindowInteractor.h>
#include <QList>

namespace FC
{
FCGraphInteractionStyle* FCGraphInteractionStyle::New()
{
    return new FCGraphInteractionStyle();
}

void FCGraphInteractionStyle::setGraph3DWin(FCGraph3DWindowVTK* graphWin)
{
    m_graph3dWin = graphWin;
}

void FCGraphInteractionStyle::setInteractor(vtkRenderWindowInteractor* interactor)
{
    if (interactor)
        interactor->SetInteractorStyle(this);
}

void FCGraphInteractionStyle::setAreaPickRenderer(vtkRenderer* renderer)
{
    if (renderer && m_areaPick)
        m_areaPick->setPickerRender(renderer);
}

vtkRenderWindowInteractor* FCGraphInteractionStyle::getInteractor()
{
    return this->Interactor;
}

void FCGraphInteractionStyle::setAredPickerState(bool state)
{
    if (m_areaPick)
        m_areaPick->enable(state);
}

void FCGraphInteractionStyle::areaPick(int* /*startPos*/, int* /*endPos*/)
{
}

void FCGraphInteractionStyle::applyDolly(double factor)
{
    Dolly(factor);
}

FCGraphInteractionStyle::FCGraphInteractionStyle()
    : QObject(nullptr)
{
    m_areaPick = new FCGraphAreaPicker(this);
}

FCGraphInteractionStyle::~FCGraphInteractionStyle()
{
    delete m_areaPick;
    m_areaPick = nullptr;
}

void FCGraphInteractionStyle::OnLeftButtonDown()
{
    if (this->Interactor)
        this->Interactor->GetEventPosition(m_leftButtonDowmPos);
    m_leftButtonPressed = true;
    vtkInteractorStyleRubberBandPick::OnLeftButtonDown();
    if (m_areaPick && m_areaPick->isEnable())
        m_areaPick->setLeftButtonDownPos(m_leftButtonDowmPos);
}

void FCGraphInteractionStyle::OnLeftButtonUp()
{
    if (this->Interactor)
        this->Interactor->GetEventPosition(m_leftButtonUpPos);
    m_leftButtonPressed = false;
    vtkInteractorStyleRubberBandPick::OnLeftButtonUp();
    if (m_areaPick)
        m_areaPick->enable(false);
}

void FCGraphInteractionStyle::OnMiddleButtonDown()
{
    if (this->Interactor)
        this->Interactor->GetEventPosition(m_leftButtonDowmPos);
    vtkInteractorStyleRubberBandPick::OnMiddleButtonDown();
}

void FCGraphInteractionStyle::OnMiddleButtonUp()
{
    if (this->Interactor)
        this->Interactor->GetEventPosition(m_leftButtonUpPos);
    vtkInteractorStyleRubberBandPick::OnMiddleButtonUp();
}

void FCGraphInteractionStyle::OnMouseMove()
{
    if (this->Interactor)
        this->Interactor->GetEventPosition(m_leftButtonUpPos);
    vtkInteractorStyleRubberBandPick::OnMouseMove();
    if (m_areaPick && isMouseMoved() && m_leftButtonPressed)
        m_areaPick->drawRectangle();
}

void FCGraphInteractionStyle::OnMouseWheelForward()
{
    vtkInteractorStyleRubberBandPick::OnMouseWheelForward();
}

void FCGraphInteractionStyle::OnMouseWheelBackward()
{
    vtkInteractorStyleRubberBandPick::OnMouseWheelBackward();
}

void FCGraphInteractionStyle::OnRightButtonDown()
{
    if (this->Interactor)
        this->Interactor->GetEventPosition(m_leftButtonDowmPos);
    vtkInteractorStyleRubberBandPick::OnRightButtonDown();
}

void FCGraphInteractionStyle::OnRightButtonUp()
{
    if (this->Interactor)
        this->Interactor->GetEventPosition(m_leftButtonUpPos);
    vtkInteractorStyleRubberBandPick::OnRightButtonUp();
}

void FCGraphInteractionStyle::OnChar()
{
    vtkRenderWindowInteractor* interactor = this->Interactor;
    if (!interactor)
        return;
    std::string curKey = interactor->GetKeySym();
    QList<std::string> keys = { "KP_3", "3", "r" };
    if (keys.contains(curKey))
        return;
    vtkInteractorStyleRubberBandPick::OnChar();
}

bool FCGraphInteractionStyle::isMouseMoved()
{
    int dx = m_leftButtonUpPos[0] - m_leftButtonDowmPos[0];
    int dy = m_leftButtonUpPos[1] - m_leftButtonDowmPos[1];
    return dx * dx + dy * dy > 50;
}
} // namespace FC
