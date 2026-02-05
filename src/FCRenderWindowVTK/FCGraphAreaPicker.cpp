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

namespace Comp
{
FCGraphAreaPicker::FCGraphAreaPicker(FCGraphInteractionStyle* style)
    : m_style(style)
{
}

FCGraphAreaPicker::~FCGraphAreaPicker() = default;

void FCGraphAreaPicker::setLeftButtonDownPos(int* pos)
{
    m_startPos[0] = pos[0];
    m_startPos[1] = pos[1];
}

void FCGraphAreaPicker::setPickerRender(vtkRenderer* render)
{
    if (!render)
        return;
    m_render = render;
    initRectangle();
    if (m_pickActor)
        m_render->AddViewProp(m_pickActor);
}

void FCGraphAreaPicker::enable(bool state)
{
    if (!m_pickActor)
        return;
    m_pickActor->SetVisibility(state ? 1 : 0);
    if (state && m_pickData)
    {
        vtkPoints* points = m_pickData->GetPoints();
        points->SetNumberOfPoints(0);
        points->InsertPoint(0, 0, 0, 0);
        points->InsertPoint(1, 0, 0, 0);
        points->InsertPoint(2, 0, 0, 0);
        points->InsertPoint(3, 0, 0, 0);
        points->Modified();
        m_pickData->Modified();
        if (m_pickActor->GetMapper())
            m_pickActor->GetMapper()->Update();
    }
    if (m_render && m_render->GetRenderWindow())
        m_render->GetRenderWindow()->Render();
}

bool FCGraphAreaPicker::isEnable()
{
    return m_pickActor && m_pickActor->GetVisibility();
}

void FCGraphAreaPicker::drawRectangle()
{
    if (!isEnable() || !m_style || !m_style->getInteractor())
        return;
    m_style->getInteractor()->GetEventPosition(m_endPos);
    updateRectangle();
}

void FCGraphAreaPicker::pick()
{
    if (m_style)
        m_style->areaPick(m_startPos, m_endPos);
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

    m_pickData = vtkSmartPointer<vtkPolyData>::New();
    m_pickData->SetPoints(points);
    m_pickData->SetLines(cells);

    vtkSmartPointer<vtkPolyDataMapper2D> mapper = vtkSmartPointer<vtkPolyDataMapper2D>::New();
    mapper->SetInputData(m_pickData);

    m_pickActor = vtkSmartPointer<vtkActor2D>::New();
    m_pickActor->SetMapper(mapper);
    m_pickActor->GetProperty()->SetColor(1, 1, 1);
    m_pickActor->GetProperty()->SetLineWidth(2);
    m_pickActor->GetProperty()->SetColor(m_color3[0], m_color3[1], m_color3[2]);
}

void FCGraphAreaPicker::updateRectangle()
{
    if (!m_render || !m_pickData || !m_pickActor)
        return;
    vtkPoints* points = m_pickData->GetPoints();
    points->SetNumberOfPoints(0);
    const int xmax = m_startPos[0] > m_endPos[0] ? m_startPos[0] : m_endPos[0];
    const int xmin = m_startPos[0] + m_endPos[0] - xmax;
    const int ymax = m_startPos[1] > m_endPos[1] ? m_startPos[1] : m_endPos[1];
    const int ymin = m_startPos[1] + m_endPos[1] - ymax;
    if (xmax - xmin < 2 || ymax - ymin < 2)
        return;
    points->InsertPoint(0, xmax, ymax, 0);
    points->InsertPoint(1, xmin, ymax, 0);
    points->InsertPoint(2, xmin, ymin, 0);
    points->InsertPoint(3, xmax, ymin, 0);
    points->Modified();
    m_pickData->Modified();
    if (m_pickActor->GetMapper())
        m_pickActor->GetMapper()->Update();
    if (m_render->GetRenderWindow())
        m_render->GetRenderWindow()->Render();
}

void FCGraphAreaPicker::setColor(double rf, double gf, double bf)
{
    m_color3[0] = rf;
    m_color3[1] = gf;
    m_color3[2] = bf;
    if (m_pickActor && m_pickActor->GetProperty())
        m_pickActor->GetProperty()->SetColor(rf, gf, bf);
}
} // namespace Comp
