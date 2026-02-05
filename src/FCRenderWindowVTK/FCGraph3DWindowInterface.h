/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCGraph3DWindowInterface.h
 * @brief 三维渲染窗口组件接口（仅 VTK+Qt，与 FITKGraph3DWindowInterface 接口一致）
 */
#ifndef FCGRAPH3DWINDOWINTERFACE_H
#define FCGRAPH3DWINDOWINTERFACE_H

#include "FCRenderWindowVTKAPI.h"
#include <functional>
#include <QHash>
#include <QVariant>
#include <QString>

#define VTKRENDERLAYER "VTKRENDERLAYER"
#define BackGroundColorTop "BackGroundColorTop"
#define BackGroundColorButtom "BackGroundColorButtom"

namespace Comp
{
class FCGraphInteractionStyle;

enum ResolveCoincidentType
{
    RCT_Off = -1,
    RCT_Default = 0,
    RCT_PolygonOffset,
    RCT_ShiftZBuffer
};

/**
 * @brief 渲染窗口信息（仅 Qt，无 FITK 依赖）
 */
class FCRENDERWINDOWVTK_API Graph3DWindowInitializer
{
public:
    explicit Graph3DWindowInitializer() = default;
    virtual ~Graph3DWindowInitializer() = default;

    virtual FCGraphInteractionStyle* getStyle();
    void setLayerCount(int nc);
    int getLayerCount() const;

    QVariant getValue(const QString& key) const;
    void setValue(const QString& key, const QVariant& value);

private:
    int _layerCount = 3;
    QHash<QString, QVariant> _params;
};

/**
 * @brief 三维渲染窗口组件接口
 */
class FCRENDERWINDOWVTK_API FCGraph3DWindowInterface
{
public:
    FCGraph3DWindowInterface() = default;
    virtual ~FCGraph3DWindowInterface();

    static QString CheckSystem();

    virtual QWidget* getWidget(int indexPort);
    virtual QString getComponentName();
    void addInitializer(int key, Graph3DWindowInitializer* info);
    static void SetResolveCoincidentType(ResolveCoincidentType type);

private:
    QHash<int, Graph3DWindowInitializer*> _initilizerList;
};
} // namespace Comp
#endif
