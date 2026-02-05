/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 */

#include "FCGraphObjectVTK.h"
#include "FCActorClipTool.h"
#include <vtkProp.h>
#include <vtkPlane.h>
#include <vtkInteractorObserver.h>

namespace FC
{
FCGraphObjectVTK::FCGraphObjectVTK(void* dataObject)
    : FCAbstractGraphObject(dataObject)
{
    mClipPlane = vtkPlane::New();
}

FCGraphObjectVTK::~FCGraphObjectVTK()
{
    removeFromGraphWidget();

    for (auto actor : mActorList)
    {
        if (actor)
        {
            actor->Delete();
        }
    }
    mActorList.clear();

    for (vtkInteractorObserver* widget : mWidgetList)
    {
        if (widget)
        {
            widget->Off();
            widget->Delete();
        }
    }
    mWidgetList.clear();

    if (mClipPlane)
    {
        mClipPlane->Delete();
        mClipPlane = nullptr;
    }
}

void FCGraphObjectVTK::setClipType(GraphClipType type)
{
    if (!mClipPlane)
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

    for (vtkProp* prop : mActorList)
    {
        FCActorClipTool* fActor = dynamic_cast<FCActorClipTool*>(prop);
        if (!fActor)
            continue;
        fActor->setClipType(cType);
        fActor->setClipImplicitFunction(mClipPlane);
    }
}

void FCGraphObjectVTK::setCustomClipPlane(double* org, double* nor)
{
    if (mClipPlane)
    {
        mClipPlane->SetOrigin(org);
        mClipPlane->SetNormal(nor);
    }
}

void FCGraphObjectVTK::addActor(vtkProp* actor)
{
    if (!actor || mActorList.contains(actor))
        return;
    mActorList.append(actor);
}

vtkProp* FCGraphObjectVTK::getActor(int index)
{
    if (index < 0 || index >= mActorList.size())
        return nullptr;
    return mActorList.at(index);
}

int FCGraphObjectVTK::getActorCount()
{
    return mActorList.size();
}

void FCGraphObjectVTK::addWidget(vtkInteractorObserver* widget)
{
    if (!widget || mWidgetList.contains(widget))
        return;
    mWidgetList.push_back(widget);
}

vtkInteractorObserver* FCGraphObjectVTK::getWidget(int index)
{
    if (index < 0 || index >= mWidgetList.count())
        return nullptr;
    return mWidgetList[index];
}

int FCGraphObjectVTK::getWidgetCount()
{
    return mWidgetList.count();
}

bool FCGraphObjectVTK::hasFixedBounds()
{
    return mHasFixedBounds;
}

bool FCGraphObjectVTK::getFixedBounds(double* bounds)
{
    if (!mHasFixedBounds)
        return false;
    double bds[6] = { 9e64, -9e64, 9e64, -9e64, 9e64, -9e64 };
    bool hasBds = getActorsBounds(mActorList, bds);
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
} // namespace FC
