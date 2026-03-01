/**
 * @file FCGeometryPropertyPanelOperator.h
 * @brief 几何属性面板操作器（独立订阅事件，可选使用）
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 *
 * @deprecated 推荐使用 FCPropertyPanelOperator + FCGeometryPropertyContentHandler 的协调者模式，
 * 便于扩展网格/物理场等（见 FCPropertyPanelOperator）。本类保留以兼容或单独使用几何逻辑。
 */
#ifndef FCGEOMETRYPROPERTYPANELOPERATOR_H
#define FCGEOMETRYPROPERTYPANELOPERATOR_H

#include "FCAbstractOperator.h"
#include "FCGeometryPropertyContentFactory.h"
#include "FCActionsOperatorAPI.h"
#include <FCData/FCType.h>
#include <QList>
#include <QVariantMap>

class QAction;

namespace FC
{
class FCIEventBus;

/**
 * @brief 几何属性面板操作器
 * 订阅事件总线，在收到几何节点选中/取消选中时执行逻辑：创建 BuildBar+InfoWidget、接线、设置到属性窗口。
 */
class FCACTIONSOPERATOR_API FCGeometryPropertyPanelOperator : public FCAbstractOperator
{
    Q_OBJECT
public:
    explicit FCGeometryPropertyPanelOperator(QObject* parent = nullptr);

    /** 设置事件总线后，本操作器订阅 EventGeometryNodeSelected / EventNoEntitySelected 并响应 */
    void setEventBus(FCIEventBus* bus);
    /** 设置构建栏要展示的全局 actions，由 APP 层注入 */
    void setBuildBarGlobalActions(const QList<QAction*>& actions);

    void onEvent(int eventType, const QVariantMap& data) override;

private slots:
    void onEventReceived(int eventType, QVariantMap data);

private:
    void handleGeometryNodeSelected(FCID nodeId);
    void handleNoEntitySelected();
    void subscribeToEventBus();

    FCIEventBus* m_eventBus{ nullptr };
    QList<QAction*> m_buildBarGlobalActions;
    FCGeometryPropertyContentFactory m_contentFactory;
};

} // namespace FC

#endif // FCGEOMETRYPROPERTYPANELOPERATOR_H
