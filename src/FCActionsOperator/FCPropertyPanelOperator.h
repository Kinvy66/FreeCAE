/**
 * @file FCPropertyPanelOperator.h
 * @brief 属性面板协调者操作器：根据工程树选中项类型（几何/网格/物理场等）调度对应 Handler，COMSOL 式
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 *
 * 只负责订阅事件、按 entityType 分发；具体内容由各 IPropertyPanelContentHandler 实现。
 */
#ifndef FCPROPERTYPANELOPERATOR_H
#define FCPROPERTYPANELOPERATOR_H

#include "FCAbstractOperator.h"
#include "IPropertyPanelContentHandler.h"
#include "FCActionsOperatorAPI.h"
#include <FCEventTypes.h>
#include <FCData/FCType.h>
#include <QHash>
#include <QVariantMap>

namespace FC
{
class FCIEventBus;

/**
 * @brief 属性面板操作器（协调者）
 * 订阅 EventTreeNodeSelected / EventNoEntitySelected，根据 entityType 调用已注册的 Handler 显示内容或清空面板。
 */
class FCACTIONSOPERATOR_API FCPropertyPanelOperator : public FCAbstractOperator
{
    Q_OBJECT
public:
    explicit FCPropertyPanelOperator(QObject* parent = nullptr);

    void setEventBus(FCIEventBus* bus);
    /** 注册某类型的内容处理器（如 Geometry -> 几何属性 Handler） */
    void registerHandler(PropertyPanelEntityType entityType, IPropertyPanelContentHandler* handler);

    void onEvent(int eventType, const QVariantMap& data) override;

private slots:
    void onEventReceived(int eventType, QVariantMap data);

private:
    void subscribeToEventBus();

    FCIEventBus* m_eventBus{ nullptr };
    QHash<int, IPropertyPanelContentHandler*> m_handlers;  // PropertyPanelEntityType -> handler
};

} // namespace FC

#endif // FCPROPERTYPANELOPERATOR_H
