/**
 * @file FCPropertyPanelOperator.cpp
 * @brief 属性面板协调者操作器实现
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#include "FCPropertyPanelOperator.h"
#include <FCDockingAreaInterface.h>
#include <FCEventTypes.h>
#include <FCIEventBus.h>
#include <FCPropertyWidget.h>

namespace FC
{

FCPropertyPanelOperator::FCPropertyPanelOperator(QObject* parent)
    : FCAbstractOperator(parent)
{
}

void FCPropertyPanelOperator::setEventBus(FCIEventBus* bus)
{
    if (m_eventBus == bus) return;
    m_eventBus = bus;
    subscribeToEventBus();
}

void FCPropertyPanelOperator::registerHandler(PropertyPanelEntityType entityType, IPropertyPanelContentHandler* handler)
{
    if (handler)
        m_handlers[static_cast<int>(entityType)] = handler;
}

void FCPropertyPanelOperator::subscribeToEventBus()
{
    if (!m_eventBus) return;
    connect(m_eventBus, &FCIEventBus::eventReceived, this, &FCPropertyPanelOperator::onEventReceived, Qt::UniqueConnection);
}

void FCPropertyPanelOperator::onEventReceived(int eventType, QVariantMap data)
{
    onEvent(eventType, data);
}

void FCPropertyPanelOperator::onEvent(int eventType, const QVariantMap& data)
{
    FCDockingAreaInterface* docking = dockingArea();
    if (!docking) return;

    if (eventType == EventTreeNodeSelected) {
        bool ok = false;
        const quint64 nodeId = data.value(QLatin1String(EventPayloadKey_NodeId)).toULongLong(&ok);
        const int entityType = data.value(QLatin1String(EventPayloadKey_EntityType)).toInt();
        if (ok) {
            IPropertyPanelContentHandler* handler = m_handlers.value(entityType);
            if (handler)
                handler->showContent(docking, static_cast<FCID>(nodeId));
        }
    } else if (eventType == EventNoEntitySelected) {
        FCPropertyWidget* propWidget = docking->getSettingParametersWidget();
        if (propWidget)
            propWidget->setContentWidget(nullptr);
    }
}

} // namespace FC
