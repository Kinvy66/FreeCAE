/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCGraphObjectVTK.h
 * @brief 渲染对象接口声明（仅 VTK+Qt，与 FITKGraphObjectVTK 接口一致）
 */
#ifndef FCGRAPHOBJECTVTK_H
#define FCGRAPHOBJECTVTK_H

#include "FCRenderWindowVTKAPI.h"
#include "FCAbstractGraphObject.h"
#include <QObject>

class vtkProp;
class vtkPlane;
class vtkInteractorObserver;

namespace FC
{
/**
 * @brief 渲染对象接口声明
 */
class FCRENDERWINDOWVTK_API FCGraphObjectVTK : public FCAbstractGraphObject
{
    Q_OBJECT
public:
    enum GraphClipType
    {
        NoneType = -1,
        Clip = 0,
        Cut,
        ExtractGeometry
    };

    explicit FCGraphObjectVTK(void* dataObject = nullptr);
    ~FCGraphObjectVTK() override;

    virtual void setClipType(GraphClipType type);
    virtual void setCustomClipPlane(double* org, double* nor);

    virtual void addActor(vtkProp* actor);
    vtkProp* getActor(int index);
    virtual int getActorCount();

    virtual void addWidget(vtkInteractorObserver* widget);
    vtkInteractorObserver* getWidget(int index);
    int getWidgetCount();

    bool hasFixedBounds();
    virtual bool getFixedBounds(double* bounds);
    bool getActorBounds(vtkProp* prop, double* bounds, bool ignoreVisibility = false);
    bool getActorsBounds(QList<vtkProp*> props, double* bounds);

protected:
    QList<vtkProp*> m_actorList;
    QList<vtkInteractorObserver*> m_widgetList;
    bool m_hasFixedBounds = true;
    double m_polygonOffset = 0.;
    vtkPlane* m_planeFunction = nullptr;
    bool m_isOpenClip = false;
    vtkPlane* m_clipPlane = nullptr;
};
} // namespace FC
#endif
