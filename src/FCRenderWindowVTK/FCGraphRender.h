/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCGraphRender.h
 * @brief 渲染场景接口声明（仅 VTK+Qt，与 FITKGraphRender 接口一致）
 */
#ifndef FCGRAPHRENDER_H
#define FCGRAPHRENDER_H

#include "FCRenderWindowVTKAPI.h"
#include <QObject>
#include <QMutex>

class vtkRenderer;
class vtkRenderWindow;

namespace Comp
{
class FCGraphObjectVTK;
class FCGraph3DWindowVTK;
class FCGraphObjManager;

class FCRENDERWINDOWVTK_API FCGraphRender : public QObject
{
    Q_OBJECT
public:
    FCGraphRender(FCGraph3DWindowVTK* graphWin, vtkRenderer* ren = nullptr);
    ~FCGraphRender() override;

    void addObject(FCGraphObjectVTK* object);
    void removeObject(FCGraphObjectVTK* object);
    vtkRenderer* getRenderer();
    FCGraphObjManager* getGraphObjManager();
    double getActorBounds(double* bound);
    int getGraphObjectCount();
    void clear();
    void setBackgroundColor(float* rgb1, float* rgb2 = nullptr);

private:
    FCGraph3DWindowVTK* _graphWidget = nullptr;
    vtkRenderer* m_renderer = nullptr;
    FCGraphObjManager* m_objectManager = nullptr;
};
} // namespace Comp
#endif
