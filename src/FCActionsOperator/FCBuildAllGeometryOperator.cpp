/**
 * @file FCBuildAllGeometryOperator.cpp
 * @brief 构建所有几何体操作器：从属性窗口取当前几何内容并执行「全部构建」
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#include "FCBuildAllGeometryOperator.h"
#include <FCDockingAreaInterface.h>
#include <FCPropertyWidget.h>
#include <FCGUIWidget/FCGeometryProperty/IGeometryBuildContent.h>
#include <QWidget>
#include <QLayout>

namespace FC
{

FCBuildAllGeometryOperator::FCBuildAllGeometryOperator()
{
}

FCDockingAreaInterface* FCBuildAllGeometryOperator::getDockingArea() const
{
    return m_docking ? m_docking : dockingArea();
}

bool FCBuildAllGeometryOperator::execGUI()
{
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
    buildContent->executeBuildAll();
    return true;
}

bool FCBuildAllGeometryOperator::execProfession()
{
    return true;
}

IGeometryBuildContent* FCBuildAllGeometryOperator::findGeometryBuildContent(QWidget* w)
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
