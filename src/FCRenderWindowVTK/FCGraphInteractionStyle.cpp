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
    mGraph3DWin = graphWin;
}

void FCGraphInteractionStyle::setInteractor(vtkRenderWindowInteractor* interactor)
{
    if (interactor)
        interactor->SetInteractorStyle(this);
}

void FCGraphInteractionStyle::setAreaPickRenderer(vtkRenderer* renderer)
{
    if (renderer && mAreaPick)
        mAreaPick->setPickerRender(renderer);
}

vtkRenderWindowInteractor* FCGraphInteractionStyle::getInteractor()
{
    return this->Interactor;
}

void FCGraphInteractionStyle::setAredPickerState(bool state)
{
    if (mAreaPick)
        mAreaPick->enable(state);
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
    mAreaPick = new FCGraphAreaPicker(this);
}

FCGraphInteractionStyle::~FCGraphInteractionStyle()
{
    delete mAreaPick;
    mAreaPick = nullptr;
}

void FCGraphInteractionStyle::OnLeftButtonDown()
{
    if (this->Interactor)
        this->Interactor->GetEventPosition(mLeftButtonDownPos);
    mLeftButtonPressed = true;
    vtkInteractorStyleRubberBandPick::OnLeftButtonDown();
    if (mAreaPick && mAreaPick->isEnable())
        mAreaPick->setLeftButtonDownPos(mLeftButtonDownPos);
}

void FCGraphInteractionStyle::OnLeftButtonUp()
{
    if (this->Interactor)
        this->Interactor->GetEventPosition(mLeftButtonUpPos);
    mLeftButtonPressed = false;
    vtkInteractorStyleRubberBandPick::OnLeftButtonUp();
    if (mAreaPick)
        mAreaPick->enable(false);
}

void FCGraphInteractionStyle::OnMiddleButtonDown()
{
    if (this->Interactor)
        this->Interactor->GetEventPosition(mLeftButtonDownPos);
    vtkInteractorStyleRubberBandPick::OnMiddleButtonDown();
}

void FCGraphInteractionStyle::OnMiddleButtonUp()
{
    if (this->Interactor)
        this->Interactor->GetEventPosition(mLeftButtonUpPos);
    vtkInteractorStyleRubberBandPick::OnMiddleButtonUp();
}

void FCGraphInteractionStyle::OnMouseMove()
{
    if (this->Interactor)
        this->Interactor->GetEventPosition(mLeftButtonUpPos);
    vtkInteractorStyleRubberBandPick::OnMouseMove();
    if (mAreaPick && isMouseMoved() && mLeftButtonPressed)
        mAreaPick->drawRectangle();
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
        this->Interactor->GetEventPosition(mLeftButtonDownPos);
    vtkInteractorStyleRubberBandPick::OnRightButtonDown();
}

void FCGraphInteractionStyle::OnRightButtonUp()
{
    if (this->Interactor)
        this->Interactor->GetEventPosition(mLeftButtonUpPos);
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
    int dx = mLeftButtonUpPos[0] - mLeftButtonDownPos[0];
    int dy = mLeftButtonUpPos[1] - mLeftButtonDownPos[1];
    return dx * dx + dy * dy > 50;
}
} // namespace FC
