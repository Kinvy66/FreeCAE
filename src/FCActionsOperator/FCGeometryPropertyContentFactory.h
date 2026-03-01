/**
 * @file FCGeometryPropertyContentFactory.h
 * @brief 几何属性面板内容工厂：根据节点类型创建 BuildBar+InfoWidget，并完成与 GraphPreprocess 的接线
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#ifndef FCGEOMETRYPROPERTYCONTENTFACTORY_H
#define FCGEOMETRYPROPERTYCONTENTFACTORY_H

#include "FCActionsOperatorAPI.h"
#include <FCData/FCType.h>
#include <QList>
#include <QWidget>

class QAction;

namespace FC
{
class FCGeometryBuildBar;
class FCGeometryDAGData;
class FCDockingAreaInterface;
enum class FCGeoOpType;

/** 工厂创建的内容：容器（用于 setContentWidget）+ BuildBar（供 Connector 接到 Build Operator） */
struct FCACTIONSOPERATOR_API GeometryPropertyContent
{
    QWidget* container{ nullptr };
    FCGeometryBuildBar* buildBar{ nullptr };
};

/**
 * @brief 几何属性内容工厂
 * 负责：按节点类型创建 BuildBar + 对应 *InfoWidget，将 InfoWidget 与 GraphPreprocess 连接并设置 DAG；
 * 不负责 BuildBar 与 BuildGeometryOperator 的接线（由 Connector 完成）。
 */
class FCACTIONSOPERATOR_API FCGeometryPropertyContentFactory
{
public:
    FCGeometryPropertyContentFactory() = default;

    /**
     * @brief 创建几何属性面板内容（BuildBar + InfoWidget 容器），并完成 InfoWidget ↔ GraphPreprocess 接线
     * @param parent 父控件（通常为属性窗口）
     * @param nodeType 几何节点类型
     * @param dagData 几何 DAG 数据
     * @param nodeId 节点 ID
     * @param docking Docking 接口（供 Operator 使用）
     * @param buildBarGlobalActions 构建栏上要展示的全局 actions
     */
    GeometryPropertyContent createContent(
        QWidget* parent,
        FCGeoOpType nodeType,
        FCGeometryDAGData* dagData,
        FCID nodeId,
        FCDockingAreaInterface* docking,
        const QList<QAction*>& buildBarGlobalActions) const;

    /**
     * @brief 在已有内容上仅更新 DAG 节点（同类型节点切换时复用当前 Widget，只刷新数据）
     * @param container 此前 createContent 返回的 container
     * @param dagData 几何 DAG 数据
     * @param nodeId 节点 ID
     * @param nodeType 几何节点类型
     */
    void updateDAGNode(
        QWidget* container,
        FCGeometryDAGData* dagData,
        FCID nodeId,
        FCGeoOpType nodeType) const;
};

} // namespace FC

#endif // FCGEOMETRYPROPERTYCONTENTFACTORY_H
