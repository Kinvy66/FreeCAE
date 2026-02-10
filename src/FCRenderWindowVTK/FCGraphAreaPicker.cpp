/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 */

#include "FCGraphAreaPicker.h"
#include "FCGraphInteractionStyle.h"
#include <vtkRenderer.h>
#include <vtkActor2D.h>
#include <vtkPoints.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper2D.h>
#include <vtkProperty2D.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCellArray.h>

namespace FC
{
FCGraphAreaPicker::FCGraphAreaPicker(FCGraphInteractionStyle* style)
    : mStyle(style)
{
}

FCGraphAreaPicker::~FCGraphAreaPicker() = default;

void FCGraphAreaPicker::setLeftButtonDownPos(int* pos)
{
    mStartPos[0] = pos[0];
    mStartPos[1] = pos[1];
}

void FCGraphAreaPicker::setPickerRender(vtkRenderer* render)
{
    if (!render)
        return;
    mRender = render;
    initRectangle();
    if (mPickActor)
    {
        mRender->AddViewProp(mPickActor);
        mPickActor->SetVisibility(0);  // 初始为禁用，避免首次左键拖动误显示框选矩形
    }
}

void FCGraphAreaPicker::enable(bool state)
{
    if (!mPickActor)
        return;
    mPickActor->SetVisibility(state ? 1 : 0);
    if (state && mPickData)
    {
        vtkPoints* points = mPickData->GetPoints();
        points->SetNumberOfPoints(0);
        points->InsertPoint(0, 0, 0, 0);
        points->InsertPoint(1, 0, 0, 0);
        points->InsertPoint(2, 0, 0, 0);
        points->InsertPoint(3, 0, 0, 0);
        points->Modified();
        mPickData->Modified();
        if (mPickActor->GetMapper())
            mPickActor->GetMapper()->Update();
    }
    if (mRender && mRender->GetRenderWindow())
        mRender->GetRenderWindow()->Render();
}

bool FCGraphAreaPicker::isEnable()
{
    return mPickActor && mPickActor->GetVisibility();
}

void FCGraphAreaPicker::drawRectangle()
{
    if (!isEnable() || !mStyle || !mStyle->getInteractor())
        return;
    mStyle->getInteractor()->GetEventPosition(mEndPos);
    updateRectangle();
}

void FCGraphAreaPicker::pick()
{
    if (mStyle)
        mStyle->areaPick(mStartPos, mEndPos);
}

void FCGraphAreaPicker::initRectangle()
{
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    for (int i = 0; i < 4; i++)
        points->InsertNextPoint(i, i, i);

    vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
    for (int i = 0; i < 4; i++)
    {
        vtkIdType line[2] = { static_cast<vtkIdType>(i), static_cast<vtkIdType>((i + 1) % 4) };
        cells->InsertNextCell(2, line);
    }

    mPickData = vtkSmartPointer<vtkPolyData>::New();
    mPickData->SetPoints(points);
    mPickData->SetLines(cells);

    vtkSmartPointer<vtkPolyDataMapper2D> mapper = vtkSmartPointer<vtkPolyDataMapper2D>::New();
    mapper->SetInputData(mPickData);

    mPickActor = vtkSmartPointer<vtkActor2D>::New();
    mPickActor->SetMapper(mapper);
    mPickActor->GetProperty()->SetColor(1, 1, 1);
    mPickActor->GetProperty()->SetLineWidth(2);
    mPickActor->GetProperty()->SetColor(mColor3[0], mColor3[1], mColor3[2]);
}

void FCGraphAreaPicker::updateRectangle()
{
    if (!mRender || !mPickData || !mPickActor)
        return;
    vtkPoints* points = mPickData->GetPoints();
    points->SetNumberOfPoints(0);
    const int xmax = mStartPos[0] > mEndPos[0] ? mStartPos[0] : mEndPos[0];
    const int xmin = mStartPos[0] + mEndPos[0] - xmax;
    const int ymax = mStartPos[1] > mEndPos[1] ? mStartPos[1] : mEndPos[1];
    const int ymin = mStartPos[1] + mEndPos[1] - ymax;
    if (xmax - xmin < 2 || ymax - ymin < 2)
        return;
    points->InsertPoint(0, xmax, ymax, 0);
    points->InsertPoint(1, xmin, ymax, 0);
    points->InsertPoint(2, xmin, ymin, 0);
    points->InsertPoint(3, xmax, ymin, 0);
    points->Modified();
    mPickData->Modified();
    if (mPickActor->GetMapper())
        mPickActor->GetMapper()->Update();
    if (mRender->GetRenderWindow())
        mRender->GetRenderWindow()->Render();
}

void FCGraphAreaPicker::setColor(double rf, double gf, double bf)
{
    mColor3[0] = rf;
    mColor3[1] = gf;
    mColor3[2] = bf;
    if (mPickActor && mPickActor->GetProperty())
        mPickActor->GetProperty()->SetColor(rf, gf, bf);
}
} // namespace FC
