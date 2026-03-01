/**
 * @file FCGeometryPropertyConnector.h
 * @brief 几何属性连接器：监听工程树几何节点选中，在属性窗口显示构建栏+属性并响应构建刷新 VTK
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 *
 * 将“几何选中 → 属性面板”“构建 → VTK 刷新”等领域逻辑从 Docking 布局类中剥离。
 * 内容创建与 InfoWidget↔GraphPreprocess 接线由 FCGeometryPropertyContentFactory 负责，
 * 本类仅负责监听树、调用工厂、将 BuildBar 接到 Build Operator、设置面板内容。
 */
#ifndef FCGEOMETRYPROPERTYCONNECTOR_H
#define FCGEOMETRYPROPERTYCONNECTOR_H

#include "FCGeometryPropertyContentFactory.h"
#include "FCPropertyPanelConnectorInterface.h"
#include "FCActionsOperatorAPI.h"
#include <FCData/FCType.h>
#include <QList>
#include <QObject>
#include <QVariantMap>

class QAction;

namespace FC
{
class FCDockingAreaInterface;
class FCIEventBus;

/**
 * @brief 几何属性连接器：实现属性面板连接器接口，专责几何节点选中时的属性面板联动
 */
class FCACTIONSOPERATOR_API FCGeometryPropertyConnector : public FCPropertyPanelConnectorInterface
{
    Q_OBJECT
public:
    explicit FCGeometryPropertyConnector(QObject* parent = nullptr);
    ~FCGeometryPropertyConnector() override = default;

    void setDockingArea(FCDockingAreaInterface* docking) override;

    /** 设置事件总线，本连接器通过订阅总线事件（如几何节点选中）更新属性面板，而非直接连树 */
    void setEventBus(FCIEventBus* bus);

    /** 设置构建栏要展示的全局 actions（如「构建所有」），由 APP 层注入 */
    void setBuildBarGlobalActions(const QList<QAction*>& actions);

private slots:
    void onEventReceived(int eventType, QVariantMap data);
    void onGeometryNodeSelected(FCID nodeId);
    void onNoEntitySelected();

private:
    void subscribeToEventBus();

    FCDockingAreaInterface* m_docking{ nullptr };
    FCIEventBus* m_eventBus{ nullptr };
    QList<QAction*> m_buildBarGlobalActions;
    FCGeometryPropertyContentFactory m_contentFactory;
};

} // namespace FC

#endif // FCGEOMETRYPROPERTYCONNECTOR_H
