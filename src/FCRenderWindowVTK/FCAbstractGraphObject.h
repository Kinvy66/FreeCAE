/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCAbstractGraphObject.h
 * @brief 渲染对象抽象基类（仅依赖 Qt，与 FITKAbstractGraphObject 接口一致）
 */
#ifndef FCABSTRACTGRAPEOBJECT_H
#define FCABSTRACTGRAPEOBJECT_H

#include "FCRenderWindowVTKAPI.h"
#include <QObject>

namespace Comp
{
class FCGraph3DWindowVTK;

/**
 * @brief 渲染对象抽象基类
 */
class FCRENDERWINDOWVTK_API FCAbstractGraphObject : public QObject
{
    Q_OBJECT
public:
    explicit FCAbstractGraphObject(void* dataObject = nullptr);
    virtual ~FCAbstractGraphObject();

    void setGraphWidget(FCGraph3DWindowVTK* w);
    FCGraph3DWindowVTK* getGraphWidget() const;
    virtual void removeFromGraphWidget();

protected:
    FCGraph3DWindowVTK* m_graphWidget = nullptr;
    void* m_dataObject = nullptr;
};
} // namespace Comp
#endif
