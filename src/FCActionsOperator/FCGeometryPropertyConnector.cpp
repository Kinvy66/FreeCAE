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
#include <FCSphereInfoWidget.h>
#include <FCCylinderInfoWidget.h>
#include <FCData/FCGlobalData.h>
#include <FCGeometryEntity/FCGeometryDAGData.h>
#include <FCGeometryEntity/FCGeoOpType.h>
#include <FCPropertyWidget.h>
#include <QObject>
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
    connect(treeWidget, &FCProjectTreeWidget::noEntitySelected,
            this, &FCGeometryPropertyConnector::onNoEntitySelected);
}

namespace {
template<typename T>
T* findWidgetByType(QWidget* w) {
    T* c = qobject_cast<T*>(w);
    if (c) return c;
    for (QObject* child : w->children()) {
        QWidget* cw = qobject_cast<QWidget*>(child);
        if (cw) {
            T* found = findWidgetByType<T>(cw);
            if (found) return found;
        }
    }
    return nullptr;
}

void connectBuildBarAndSetDAG(FCGeometryBuildBar* buildBar, QWidget* contentWidget,
    FCGeometryDAGData* dagData, FCID nodeId, FCDockingAreaInterface* docking)
{
    FCGraphPreprocessOperator* graphOper = FCOPERATORREPO->getOperatorT<FCGraphPreprocessOperator>("GraphPreprocessEvent");
    if (graphOper) {
        graphOper->setDockingArea(docking);
        if (FCCubeInfoWidget* w = findWidgetByType<FCCubeInfoWidget>(contentWidget)) {
            QObject::connect(w, &FCCubeInfoWidget::geometryBuilt, graphOper, &FCGraphPreprocessOperator::onGeometryBuilt);
            QObject::connect(w, &FCCubeInfoWidget::geometrySequenceBuilt, graphOper, &FCGraphPreprocessOperator::onGeometrySequenceBuilt);
            w->setDAGNode(dagData, nodeId, nullptr);
        } else if (FCSphereInfoWidget* w = findWidgetByType<FCSphereInfoWidget>(contentWidget)) {
            QObject::connect(w, &FCSphereInfoWidget::geometryBuilt, graphOper, &FCGraphPreprocessOperator::onGeometryBuilt);
            QObject::connect(w, &FCSphereInfoWidget::geometrySequenceBuilt, graphOper, &FCGraphPreprocessOperator::onGeometrySequenceBuilt);
            w->setDAGNode(dagData, nodeId, nullptr);
        } else if (FCCylinderInfoWidget* w = findWidgetByType<FCCylinderInfoWidget>(contentWidget)) {
            QObject::connect(w, &FCCylinderInfoWidget::geometryBuilt, graphOper, &FCGraphPreprocessOperator::onGeometryBuilt);
            QObject::connect(w, &FCCylinderInfoWidget::geometrySequenceBuilt, graphOper, &FCGraphPreprocessOperator::onGeometrySequenceBuilt);
            w->setDAGNode(dagData, nodeId, nullptr);
        }
    }
    FCBuildGeometryOperator* buildOper = FCOPERATORREPO->getOperatorT<FCBuildGeometryOperator>("BuildGeometry");
    FCBuildAllGeometryOperator* buildAllOper = FCOPERATORREPO->getOperatorT<FCBuildAllGeometryOperator>("BuildAllGeometry");
    if (buildOper) {
        buildOper->setDockingArea(docking);
        QObject::connect(buildBar, &FCGeometryBuildBar::buildClicked, buildOper, [buildOper]() { buildOper->actionTriggered(); });
    }
    if (buildAllOper) {
        buildAllOper->setDockingArea(docking);
        QObject::connect(buildBar, &FCGeometryBuildBar::buildAllClicked, buildAllOper, [buildAllOper]() { buildAllOper->actionTriggered(); });
    }
}
} // namespace

void FCGeometryPropertyConnector::onGeometryNodeSelected(FCID nodeId)
{
    if (!m_docking || nodeId == FCID_INVALID) return;
    FCPropertyWidget* propWidget = m_docking->getSettingParametersWidget();
    if (!propWidget) return;
    FCGlobalData* globalData = FCGlobalData::getGlobalData();
    if (!globalData) return;
    FCGeometryDAGData* dagData = globalData->getGeometryData<FCGeometryDAGData>();
    if (!dagData) return;

    FCGeoOpType nodeType = dagData->module()->tree()->node(nodeId).type;

    QLayout* lay = propWidget->layout();
    if (lay && lay->count() > 0) {
        QWidget* content = lay->itemAt(0)->widget();
        bool match = false;
        if (nodeType == FCGeoOpType::Block) {
            FCCubeInfoWidget* w = content ? findWidgetByType<FCCubeInfoWidget>(content) : nullptr;
            if (w) { w->setDAGNode(dagData, nodeId, nullptr); match = true; }
        } else if (nodeType == FCGeoOpType::Sphere) {
            FCSphereInfoWidget* w = content ? findWidgetByType<FCSphereInfoWidget>(content) : nullptr;
            if (w) { w->setDAGNode(dagData, nodeId, nullptr); match = true; }
        } else if (nodeType == FCGeoOpType::Cylinder) {
            FCCylinderInfoWidget* w = content ? findWidgetByType<FCCylinderInfoWidget>(content) : nullptr;
            if (w) { w->setDAGNode(dagData, nodeId, nullptr); match = true; }
        }
        if (match) return;
    }

    FCGeometryBuildBar* buildBar = new FCGeometryBuildBar(propWidget);
    QWidget* contentWidget = nullptr;
    if (nodeType == FCGeoOpType::Block)
        contentWidget = new FCCubeInfoWidget(propWidget);
    else if (nodeType == FCGeoOpType::Sphere)
        contentWidget = new FCSphereInfoWidget(propWidget);
    else if (nodeType == FCGeoOpType::Cylinder)
        contentWidget = new FCCylinderInfoWidget(propWidget);
    else
        contentWidget = new FCCubeInfoWidget(propWidget);

    QWidget* container = new QWidget(propWidget);
    QVBoxLayout* containerLayout = new QVBoxLayout(container);
    containerLayout->setContentsMargins(0, 0, 0, 0);
    containerLayout->addWidget(buildBar);
    containerLayout->addWidget(contentWidget, 1);
    propWidget->setContentWidget(container);

    connectBuildBarAndSetDAG(buildBar, contentWidget, dagData, nodeId, m_docking);
}

void FCGeometryPropertyConnector::onNoEntitySelected()
{
    if (!m_docking) return;
    FCPropertyWidget* propWidget = m_docking->getSettingParametersWidget();
    if (propWidget)
        propWidget->setContentWidget(nullptr);
}

} // namespace FC
