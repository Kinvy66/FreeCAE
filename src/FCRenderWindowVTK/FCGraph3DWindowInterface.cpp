/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

#include "FCGraph3DWindowInterface.h"
#include "FCGraph3DWindowVTK.h"
#include "FCGraphInteractionStyle.h"
#include <vtkMapper.h>
#include <QWidget>
#include <QColor>

namespace Comp
{
QString FCGraph3DWindowInterface::CheckSystem()
{
    return QString();
}

FCGraph3DWindowInterface::~FCGraph3DWindowInterface()
{
    QList<Graph3DWindowInitializer*> inis = _initilizerList.values();
    for (auto iner : inis)
    {
        if (iner)
            delete iner;
    }
    _initilizerList.clear();
}

QWidget* FCGraph3DWindowInterface::getWidget(int indexPort)
{
    Graph3DWindowInitializer* initer = _initilizerList.value(indexPort);
    if (!initer)
        initer = new Graph3DWindowInitializer;
    FCGraph3DWindowVTK* w = new FCGraph3DWindowVTK(initer);

    QVariant vtc = initer->getValue(BackGroundColorTop);
    QVariant vbc = initer->getValue(BackGroundColorButtom);
    if (vtc.isValid() && vbc.isValid())
    {
        QColor c = vtc.value<QColor>();
        float t[3] = { c.redF(), c.greenF(), c.blueF() };
        c = vbc.value<QColor>();
        float b[3] = { c.redF(), c.greenF(), c.blueF() };
        if (w)
            w->setBackgroundColor(t, b);
    }
    return w;
}

QString FCGraph3DWindowInterface::getComponentName()
{
    return QStringLiteral("Graph3DWindowVTK");
}

void FCGraph3DWindowInterface::addInitializer(int key, Graph3DWindowInitializer* info)
{
    if (_initilizerList.contains(key))
    {
        Graph3DWindowInitializer* iner = _initilizerList.value(key);
        if (iner)
            delete iner;
    }
    _initilizerList[key] = info;
}

FCGraphInteractionStyle* Graph3DWindowInitializer::getStyle()
{
    return FCGraphInteractionStyle::New();
}

void Graph3DWindowInitializer::setLayerCount(int nc)
{
    _layerCount = nc;
}

int Graph3DWindowInitializer::getLayerCount() const
{
    return _layerCount;
}

QVariant Graph3DWindowInitializer::getValue(const QString& key) const
{
    return _params.value(key);
}

void Graph3DWindowInitializer::setValue(const QString& key, const QVariant& value)
{
    _params[key] = value;
}

void FCGraph3DWindowInterface::SetResolveCoincidentType(ResolveCoincidentType type)
{
    switch (type)
    {
    case RCT_Off:
        vtkMapper::SetResolveCoincidentTopologyToOff();
        break;
    case RCT_Default:
        vtkMapper::SetResolveCoincidentTopologyToDefault();
        break;
    case RCT_PolygonOffset:
        vtkMapper::SetResolveCoincidentTopologyToPolygonOffset();
        break;
    case RCT_ShiftZBuffer:
        vtkMapper::SetResolveCoincidentTopologyToShiftZBuffer();
        break;
    default:
        break;
    }
}
} // namespace Comp
