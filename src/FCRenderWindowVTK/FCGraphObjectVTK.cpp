/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 */

#include "FCGraphObjectVTK.h"
#include "FCActorClipTool.h"
#include <vtkProp.h>
#include <vtkPlane.h>
#include <vtkInteractorObserver.h>

namespace Comp
{
FCGraphObjectVTK::FCGraphObjectVTK(void* dataObject)
    : FCAbstractGraphObject(dataObject)
{
    m_clipPlane = vtkPlane::New();
}

FCGraphObjectVTK::~FCGraphObjectVTK()
{
    removeFromGraphWidget();

    for (auto actor : m_actorList)
    {
        if (actor)
        {
            actor->Delete();
        }
    }
    m_actorList.clear();

    for (vtkInteractorObserver* widget : m_widgetList)
    {
        if (widget)
        {
            widget->Off();
            widget->Delete();
        }
    }
    m_widgetList.clear();

    if (m_clipPlane)
    {
        m_clipPlane->Delete();
        m_clipPlane = nullptr;
    }
}

void FCGraphObjectVTK::setClipType(GraphClipType type)
{
    if (!m_clipPlane)
        return;

    ClipType cType = ClipType::NoneType;
    switch (type)
    {
    case Clip:
        cType = ClipType::Clip;
        break;
    case Cut:
        cType = ClipType::Cut;
        break;
    case ExtractGeometry:
        cType = ClipType::ExtractGeometry;
        break;
    default:
        break;
    }

    for (vtkProp* prop : m_actorList)
    {
        FCActorClipTool* fActor = dynamic_cast<FCActorClipTool*>(prop);
        if (!fActor)
            continue;
        fActor->setClipType(cType);
        fActor->setClipImplicitFunction(m_clipPlane);
    }
}

void FCGraphObjectVTK::setCustomClipPlane(double* org, double* nor)
{
    if (m_clipPlane)
    {
        m_clipPlane->SetOrigin(org);
        m_clipPlane->SetNormal(nor);
    }
}

void FCGraphObjectVTK::addActor(vtkProp* actor)
{
    if (!actor || m_actorList.contains(actor))
        return;
    m_actorList.append(actor);
}

vtkProp* FCGraphObjectVTK::getActor(int index)
{
    if (index < 0 || index >= m_actorList.size())
        return nullptr;
    return m_actorList.at(index);
}

int FCGraphObjectVTK::getActorCount()
{
    return m_actorList.size();
}

void FCGraphObjectVTK::addWidget(vtkInteractorObserver* widget)
{
    if (!widget || m_widgetList.contains(widget))
        return;
    m_widgetList.push_back(widget);
}

vtkInteractorObserver* FCGraphObjectVTK::getWidget(int index)
{
    if (index < 0 || index >= m_widgetList.count())
        return nullptr;
    return m_widgetList[index];
}

int FCGraphObjectVTK::getWidgetCount()
{
    return m_widgetList.count();
}

bool FCGraphObjectVTK::hasFixedBounds()
{
    return m_hasFixedBounds;
}

bool FCGraphObjectVTK::getFixedBounds(double* bounds)
{
    if (!m_hasFixedBounds)
        return false;
    double bds[6] = { 9e64, -9e64, 9e64, -9e64, 9e64, -9e64 };
    bool hasBds = getActorsBounds(m_actorList, bds);
    for (int i = 0; i < 6; i++)
        bounds[i] = bds[i];
    return hasBds;
}

bool FCGraphObjectVTK::getActorBounds(vtkProp* prop, double* bounds, bool ignoreVisibility)
{
    if (!prop)
        return false;
    if (!prop->GetVisibility() && !ignoreVisibility)
        return false;
    if (!prop->GetUseBounds())
        return false;
    double* bds = prop->GetBounds();
    if (!bds)
        return false;
    for (int i = 0; i < 6; i++)
        bounds[i] = bds[i];
    return true;
}

bool FCGraphObjectVTK::getActorsBounds(QList<vtkProp*> props, double* bounds)
{
    bool hasBds = false;
    double bdsAll[6] = { 9e64, -9e64, 9e64, -9e64, 9e64, -9e64 };
    for (vtkProp* prop : props)
    {
        if (!prop || !prop->GetVisibility())
            continue;
        double bds[6] = { 9e64, -9e64, 9e64, -9e64, 9e64, -9e64 };
        if (!getActorBounds(prop, bds))
            continue;
        if (bds[0] < bdsAll[0]) bdsAll[0] = bds[0];
        if (bds[1] > bdsAll[1]) bdsAll[1] = bds[1];
        if (bds[2] < bdsAll[2]) bdsAll[2] = bds[2];
        if (bds[3] > bdsAll[3]) bdsAll[3] = bds[3];
        if (bds[4] < bdsAll[4]) bdsAll[4] = bds[4];
        if (bds[5] > bdsAll[5]) bdsAll[5] = bds[5];
        hasBds = true;
    }
    for (int i = 0; i < 6; i++)
        bounds[i] = bdsAll[i];
    return hasBds;
}
} // namespace Comp
