/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 */

#include "FCAbstractGraphObject.h"
#include "FCGraph3DWindowVTK.h"

namespace FC
{
FCAbstractGraphObject::FCAbstractGraphObject(void* dataObject)
    : m_dataObject(dataObject)
{
}

FCAbstractGraphObject::~FCAbstractGraphObject() = default;

void FCAbstractGraphObject::setGraphWidget(FCGraph3DWindowVTK* w)
{
    m_graphWidget = w;
}

FCGraph3DWindowVTK* FCAbstractGraphObject::getGraphWidget() const
{
    return m_graphWidget;
}

void FCAbstractGraphObject::removeFromGraphWidget()
{
    if (m_graphWidget)
    {
        m_graphWidget->removeGraphObj(this);
        m_graphWidget = nullptr;
    }
}
} // namespace FC
