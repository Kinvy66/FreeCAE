/**
 * @file FCGeometryPropertyConnector.cpp
 * @brief 几何属性连接器实现
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#include "FCGeometryPropertyConnector.h"
#include "FCGraphPreprocessOperator.h"
#include "FCBuildGeometryOperator.h"
#include "FCBuildAllGeometryOperator.h"
#include "FCOperatorRepo.h"
#include <FCDockingAreaInterface.h>
#include <FCGUIFrame/FCMainTreeWidget.h>
#include <FCGUIWidget/FCProjectTreeWidget.h>
#include <FCGeometryBuildBar.h>
#include <FCCubeInfoWidget.h>
#include <FCData/FCGlobalData.h>
#include <FCGeometryEntity/FCGeometryDAGData.h>
#include <FCPropertyWidget.h>
#include <QVBoxLayout>

namespace FC
{

FCGeometryPropertyConnector::FCGeometryPropertyConnector(QObject* parent)
    : QObject(parent)
{
}

void FCGeometryPropertyConnector::setDockingArea(FCDockingAreaInterface* docking)
{
    if (m_docking == docking) return;
    m_docking = docking;
    if (!m_docking) return;

    FCMainTreeWidget* modelWidget = m_docking->getModelBuilderWidget();
    if (!modelWidget) return;
    FCProjectTreeWidget* treeWidget = modelWidget->getTreeWidget();
    if (!treeWidget) return;

    connect(treeWidget, &FCProjectTreeWidget::geometryNodeSelected,
            this, &FCGeometryPropertyConnector::onGeometryNodeSelected);
}

namespace {
FCCubeInfoWidget* findCubeInfoWidget(QWidget* w)
{
    FCCubeInfoWidget* c = qobject_cast<FCCubeInfoWidget*>(w);
    if (c) return c;
    for (QObject* child : w->children()) {
        QWidget* cw = qobject_cast<QWidget*>(child);
        if (cw) {
            FCCubeInfoWidget* found = findCubeInfoWidget(cw);
            if (found) return found;
        }
    }
    return nullptr;
}
} // namespace

void FCGeometryPropertyConnector::onGeometryNodeSelected(int nodeId)
{
    if (!m_docking || nodeId < 0) return;
    FCPropertyWidget* propWidget = m_docking->getSettingParametersWidget();
    if (!propWidget) return;
    FCGlobalData* globalData = FCGlobalData::getGlobalData();
    if (!globalData) return;
    FCGeometryDAGData* dagData = globalData->getGeometryData<FCGeometryDAGData>();
    if (!dagData) return;

    // 若当前属性区已是几何体表单，只重新从 DAG 读取选中节点属性并刷新页面，不重建控件
    QLayout* lay = propWidget->layout();
    if (lay && lay->count() > 0) {
        QWidget* content = lay->itemAt(0)->widget();
        FCCubeInfoWidget* cubeWidget = content ? findCubeInfoWidget(content) : nullptr;
        if (cubeWidget) {
            cubeWidget->setDAGNode(dagData, nodeId, nullptr);
            return;
        }
    }

    FCGeometryBuildBar* buildBar = new FCGeometryBuildBar(propWidget);
    FCCubeInfoWidget* cubeWidget = new FCCubeInfoWidget(propWidget);
    QWidget* container = new QWidget(propWidget);
    QVBoxLayout* containerLayout = new QVBoxLayout(container);
    containerLayout->setContentsMargins(0, 0, 0, 0);
    containerLayout->addWidget(buildBar);
    containerLayout->addWidget(cubeWidget, 1);
    propWidget->setContentWidget(container);

    FCGraphPreprocessOperator* graphOper = FCOPERATORREPO->getOperatorT<FCGraphPreprocessOperator>("GraphPreprocessEvent");
    if (graphOper) {
        graphOper->setDockingArea(m_docking);
        connect(cubeWidget, &FCCubeInfoWidget::geometryBuilt, graphOper, &FCGraphPreprocessOperator::onGeometryBuilt);
        connect(cubeWidget, &FCCubeInfoWidget::geometrySequenceBuilt, graphOper, &FCGraphPreprocessOperator::onGeometrySequenceBuilt);
    }
    FCBuildGeometryOperator* buildOper = FCOPERATORREPO->getOperatorT<FCBuildGeometryOperator>("BuildGeometry");
    FCBuildAllGeometryOperator* buildAllOper = FCOPERATORREPO->getOperatorT<FCBuildAllGeometryOperator>("BuildAllGeometry");
    if (buildOper) {
        buildOper->setDockingArea(m_docking);
        connect(buildBar, &FCGeometryBuildBar::buildClicked, this, [buildOper]() { buildOper->actionTriggered(); });
    }
    if (buildAllOper) {
        buildAllOper->setDockingArea(m_docking);
        connect(buildBar, &FCGeometryBuildBar::buildAllClicked, this, [buildAllOper]() { buildAllOper->actionTriggered(); });
    }

    cubeWidget->setDAGNode(dagData, nodeId, nullptr);
}

} // namespace FC
