/**
 * @file FCGeometryPropertyConnector.h
 * @brief 几何体与属性窗口/VTK 的桥接：工程树选中几何节点时显示属性并响应构建
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 *
 * 将“几何选中 → 属性面板”“构建 → VTK 刷新”等领域逻辑从 Docking 布局类中剥离，
 * 保持 FCAppDockingArea 仅负责布局管理。
 */
#ifndef FCGEOMETRYPROPERTYCONNECTOR_H
#define FCGEOMETRYPROPERTYCONNECTOR_H

#include "FCActionsOperatorAPI.h"
#include <FCData/FCType.h>
#include <QObject>

namespace FC
{
class FCDockingAreaInterface;

/**
 * @brief 几何属性连接器：监听工程树几何节点选中，在属性窗口显示构建栏+属性并响应构建刷新 VTK
 */
class FCACTIONSOPERATOR_API FCGeometryPropertyConnector : public QObject
{
    Q_OBJECT
public:
    explicit FCGeometryPropertyConnector(QObject* parent = nullptr);
    ~FCGeometryPropertyConnector() override = default;

    /** 设置 Docking 区域接口（树、属性窗口、图形窗口均通过其获取），并建立连接 */
    void setDockingArea(FCDockingAreaInterface* docking);

private slots:
    void onGeometryNodeSelected(FCID nodeId);
    void onNoEntitySelected();

private:
    FCDockingAreaInterface* m_docking{ nullptr };
};

} // namespace FC

#endif // FCGEOMETRYPROPERTYCONNECTOR_H
