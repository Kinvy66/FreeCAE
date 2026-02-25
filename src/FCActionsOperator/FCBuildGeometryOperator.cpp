/**
 * @file FCBuildGeometryOperator.cpp
 * @brief 几何体构建操作器：从属性窗口取当前几何内容并执行「构建」
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#include "FCBuildGeometryOperator.h"
#include <FCDockingAreaInterface.h>
#include <FCPropertyWidget.h>
#include <FCGUIWidget/FCGeometryProperty/IGeometryBuildContent.h>
#include <QWidget>
#include <QLayout>

namespace FC
{

FCBuildGeometryOperator::FCBuildGeometryOperator()
{
}

FCDockingAreaInterface* FCBuildGeometryOperator::getDockingArea() const
{
    return m_docking ? m_docking : dockingArea();
}

bool FCBuildGeometryOperator::execGUI()
{
    // TODO: VTK窗口刷新
    
    FCDockingAreaInterface* docking = getDockingArea();
    if (!docking) return false;
    FCPropertyWidget* propWidget = docking->getSettingParametersWidget();
    if (!propWidget) return false;
    QLayout* lay = propWidget->layout();
    if (!lay || lay->count() == 0) return false;
    QWidget* content = lay->itemAt(0)->widget();
    if (!content) return false;
    IGeometryBuildContent* buildContent = findGeometryBuildContent(content);
    if (!buildContent) return false;
    buildContent->executeBuild();
    return true;
}

bool FCBuildGeometryOperator::execProfession()
{
    
    // TODO: 从DAG中获取最新的几何序列，构建实际的几何模型
    // 将构建的几何模型三角化，存放到VTK对象中
    
    return true;
}

IGeometryBuildContent* FCBuildGeometryOperator::findGeometryBuildContent(QWidget* w)
{
    IGeometryBuildContent* c = dynamic_cast<IGeometryBuildContent*>(w);
    if (c) return c;
    for (QObject* child : w->children()) {
        QWidget* cw = qobject_cast<QWidget*>(child);
        if (cw && (c = findGeometryBuildContent(cw))) return c;
    }
    return nullptr;
}

} // namespace FC
