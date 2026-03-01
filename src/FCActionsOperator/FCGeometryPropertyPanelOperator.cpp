/**
 * @file FCGeometryPropertyPanelOperator.cpp
 * @brief 几何属性面板操作器实现
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#include "FCGeometryPropertyPanelOperator.h"
#include "FCBuildGeometryOperator.h"
#include "FCBuildAllGeometryOperator.h"
#include "FCOperatorRepo.h"
#include <FCGeometryBuildBar.h>
#include <FCCubeInfoWidget.h>
#include <FCSphereInfoWidget.h>
#include <FCCylinderInfoWidget.h>
#include <FCConeInfoWidget.h>
#include <FCTorusInfoWidget.h>
#include <FCHelixInfoWidget.h>
#include <FCDockingAreaInterface.h>
#include <FCEventTypes.h>
#include <FCIEventBus.h>
#include <FCData/FCGlobalData.h>
#include <FCGeometryEntity/FCGeometryDAGData.h>
#include <FCGeometryEntity/FCGeoOpType.h>
#include <FCPropertyWidget.h>
#include <QLayout>

namespace FC
{

namespace {

template<typename T>
T* findWidgetByType(QWidget* w)
{
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

void wireBuildBarToOperators(FCGeometryBuildBar* buildBar, FCDockingAreaInterface* docking)
{
    if (!buildBar || !docking) return;
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

bool isContainerForNodeType(QWidget* container, FCGeoOpType nodeType)
{
    if (!container) return false;
    QLayout* lay = container->layout();
    if (!lay || lay->count() < 2) return false;
    switch (nodeType) {
    case FCGeoOpType::Block:   return findWidgetByType<FCCubeInfoWidget>(container) != nullptr;
    case FCGeoOpType::Sphere:  return findWidgetByType<FCSphereInfoWidget>(container) != nullptr;
    case FCGeoOpType::Cylinder: return findWidgetByType<FCCylinderInfoWidget>(container) != nullptr;
    case FCGeoOpType::Cone:   return findWidgetByType<FCConeInfoWidget>(container) != nullptr;
    case FCGeoOpType::Torus:  return findWidgetByType<FCTorusInfoWidget>(container) != nullptr;
    case FCGeoOpType::Helix:  return findWidgetByType<FCHelixInfoWidget>(container) != nullptr;
    default:                   return findWidgetByType<FCCubeInfoWidget>(container) != nullptr;
    }
}

} // namespace

FCGeometryPropertyPanelOperator::FCGeometryPropertyPanelOperator(QObject* parent)
    : FCAbstractOperator(parent)
{
}

void FCGeometryPropertyPanelOperator::setEventBus(FCIEventBus* bus)
{
    if (m_eventBus == bus) return;
    m_eventBus = bus;
    subscribeToEventBus();
}

void FCGeometryPropertyPanelOperator::setBuildBarGlobalActions(const QList<QAction*>& actions)
{
    m_buildBarGlobalActions = actions;
}

void FCGeometryPropertyPanelOperator::subscribeToEventBus()
{
    if (!m_eventBus) return;
    connect(m_eventBus, &FCIEventBus::eventReceived, this, &FCGeometryPropertyPanelOperator::onEventReceived, Qt::UniqueConnection);
}

void FCGeometryPropertyPanelOperator::onEventReceived(int eventType, QVariantMap data)
{
    onEvent(eventType, data);
}

void FCGeometryPropertyPanelOperator::onEvent(int eventType, const QVariantMap& data)
{
    if (eventType == EventTreeNodeSelected) {
        if (data.value(QLatin1String(EventPayloadKey_EntityType)).toInt() != static_cast<int>(PropertyPanelEntityGeometry))
            return;
        bool ok = false;
        const quint64 id = data.value(QLatin1String(EventPayloadKey_NodeId)).toULongLong(&ok);
        if (ok) handleGeometryNodeSelected(static_cast<FCID>(id));
    } else if (eventType == EventNoEntitySelected) {
        handleNoEntitySelected();
    }
}

void FCGeometryPropertyPanelOperator::handleGeometryNodeSelected(FCID nodeId)
{
    FCDockingAreaInterface* docking = dockingArea();
    if (!docking || nodeId == FCID_INVALID) return;
    FCPropertyWidget* propWidget = docking->getSettingParametersWidget();
    if (!propWidget) return;
    FCGlobalData* globalData = FCGlobalData::getGlobalData();
    if (!globalData) return;
    FCGeometryDAGData* dagData = globalData->getGeometryData<FCGeometryDAGData>();
    if (!dagData) return;

    FCGeoOpType nodeType = dagData->module()->tree()->node(nodeId).type;

    QLayout* lay = propWidget->layout();
    if (lay && lay->count() > 0) {
        QWidget* currentContent = lay->itemAt(0)->widget();
        if (currentContent && isContainerForNodeType(currentContent, nodeType)) {
            m_contentFactory.updateDAGNode(currentContent, dagData, nodeId, nodeType);
            return;
        }
    }

    GeometryPropertyContent content = m_contentFactory.createContent(
        propWidget, nodeType, dagData, nodeId, docking, m_buildBarGlobalActions);
    if (!content.container) return;

    wireBuildBarToOperators(content.buildBar, docking);
    propWidget->setContentWidget(content.container);
}

void FCGeometryPropertyPanelOperator::handleNoEntitySelected()
{
    FCDockingAreaInterface* docking = dockingArea();
    if (!docking) return;
    FCPropertyWidget* propWidget = docking->getSettingParametersWidget();
    if (propWidget)
        propWidget->setContentWidget(nullptr);
}

} // namespace FC
