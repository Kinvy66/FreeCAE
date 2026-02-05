/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 */

#include "FCGraphRender.h"
#include "FCGraphObjectVTK.h"
#include "FCGraph3DWindowVTK.h"
#include "FCGraphObjManager.h"
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkInteractorObserver.h>
#include <vtkCubeAxesActor.h>
#include <vtkCamera.h>
#include <cmath>

namespace Comp
{
FCGraphRender::FCGraphRender(FCGraph3DWindowVTK* gw, vtkRenderer* ren)
    : _graphWidget(gw)
{
    if (!_graphWidget)
        return;
    m_objectManager = new FCGraphObjManager();

    if (ren)
        m_renderer = ren;
    else
        m_renderer = vtkRenderer::New();

    m_renderer->SetBackground2(250/255.0, 251/255.0, 254/255.0);
    m_renderer->SetBackground(230/255.0, 242/255.0, 255/255.0);
    m_renderer->SetGradientBackground(true);
    m_renderer->InteractiveOn();

    vtkRenderWindow* renderWin = _graphWidget->getVTKRenderWindow();
    if (renderWin)
        renderWin->AddRenderer(m_renderer);
}

FCGraphRender::~FCGraphRender()
{
    clear();
    if (m_objectManager)
        delete m_objectManager;
    if (m_renderer)
        m_renderer->Delete();
}

void FCGraphRender::addObject(FCGraphObjectVTK* object)
{
    if (!object)
        return;
    m_objectManager->appendGraphObj(object);
    object->setGraphWidget(_graphWidget);

    const int nact = object->getActorCount();
    if (!m_renderer)
        return;

    vtkCamera* c = m_renderer->GetActiveCamera();
    for (int i = 0; i < nact; i++)
    {
        vtkProp* act = object->getActor(i);
        if (!act)
            continue;
        vtkCubeAxesActor* cubeAxes = vtkCubeAxesActor::SafeDownCast(act);
        if (cubeAxes)
            cubeAxes->SetCamera(c);
        m_renderer->AddViewProp(act);
    }
}

void FCGraphRender::removeObject(FCGraphObjectVTK* gobj)
{
    if (!gobj || !m_objectManager->isContains(gobj))
        return;
    gobj->setGraphWidget(nullptr);
    const int nact = gobj->getActorCount();
    if (m_renderer)
    {
        for (int i = 0; i < nact; i++)
            m_renderer->RemoveViewProp(gobj->getActor(i));
        int nWidget = gobj->getWidgetCount();
        for (int i = 0; i < nWidget; i++)
        {
            vtkInteractorObserver* widget = gobj->getWidget(i);
            if (widget)
            {
                widget->SetDefaultRenderer(nullptr);
                widget->SetInteractor(nullptr);
                widget->SetEnabled(false);
            }
        }
    }
    m_objectManager->removeGraphObj(gobj);
}

vtkRenderer* FCGraphRender::getRenderer()
{
    return m_renderer;
}

FCGraphObjManager* FCGraphRender::getGraphObjManager()
{
    return m_objectManager;
}

double FCGraphRender::getActorBounds(double* bound)
{
    bool isEmpty = true;
    const int ngobj = m_objectManager->getGraphObjCount();
    for (int i = 0; i < ngobj; ++i)
    {
        FCGraphObjectVTK* object = m_objectManager->getGraphObjTAt<FCGraphObjectVTK>(i);
        if (!object)
            continue;
        double b[6] = { 9e64, -9e64, 9e64, -9e64, 9e64, -9e64 };
        if (!object->getFixedBounds(b))
            continue;
        if (b[0] < bound[0]) bound[0] = b[0];
        if (b[1] > bound[1]) bound[1] = b[1];
        if (b[2] < bound[2]) bound[2] = b[2];
        if (b[3] > bound[3]) bound[3] = b[3];
        if (b[4] < bound[4]) bound[4] = b[4];
        if (b[5] > bound[5]) bound[5] = b[5];
        isEmpty = false;
    }
    if (isEmpty)
        return -1;
    double dx2 = (bound[1] - bound[0]) * (bound[1] - bound[0]);
    double dy2 = (bound[3] - bound[2]) * (bound[3] - bound[2]);
    double dz2 = (bound[5] - bound[4]) * (bound[5] - bound[4]);
    return std::sqrt(dx2 + dy2 + dz2);
}

int FCGraphRender::getGraphObjectCount()
{
    return m_objectManager ? m_objectManager->getGraphObjCount() : 0;
}

void FCGraphRender::clear()
{
    const int nActor = m_objectManager->getGraphObjCount();
    for (int i = 0; i < nActor; ++i)
    {
        FCGraphObjectVTK* gobj = m_objectManager->getGraphObjTAt<FCGraphObjectVTK>(i);
        if (!gobj)
            continue;
        gobj->setGraphWidget(nullptr);
        const int nact = gobj->getActorCount();
        if (m_renderer)
        {
            for (int j = 0; j < nact; j++)
                m_renderer->RemoveViewProp(gobj->getActor(j));
        }
    }
    m_objectManager->clear();
}

void FCGraphRender::setBackgroundColor(float* rgb1, float* rgb2)
{
    if (!rgb1 || !m_renderer)
        return;
    if (!rgb2)
        rgb2 = rgb1;
    m_renderer->SetGradientBackground(true);
    m_renderer->SetBackground2(rgb1[0], rgb1[1], rgb1[2]);
    m_renderer->SetBackground(rgb2[0], rgb2[1], rgb2[2]);
}
} // namespace Comp
