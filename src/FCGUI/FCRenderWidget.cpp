/**
 * @file FCRenderWidget.cpp
 * @brief VTK渲染窗口容器，内嵌 FCGraph3DWindowVTK
 * @date 2026-02-04
 * @version V0.0.1
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#include "FCRenderWidget.h"
#include "FCGraph3DWindowVTK.h"
#include "FCGraph3DWindowInterface.h"
#include <QVBoxLayout>

namespace FC
{
FCRenderWidget::FCRenderWidget(QWidget* parent)
    : QWidget(parent)
{
    m_initializer = new FCGraph3DWindowInitializer();
    m_initializer->setLayerCount(3);

    m_graph3DWindow = new FCGraph3DWindowVTK(m_initializer);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_graph3DWindow);
}

FCRenderWidget::~FCRenderWidget()
{
    delete m_initializer;
    m_initializer = nullptr;
    m_graph3DWindow = nullptr; 
}
} // namespace FC
