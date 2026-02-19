/**
 * @file FCGeometryBuildEngine.h
 * @brief 构建引擎：topoSort、applyNode、nodeResults 缓存、dirty 传播与局部重建
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#ifndef FCGEOMETRYBUILDENGINE_H
#define FCGEOMETRYBUILDENGINE_H

#include "FCGeometryEntityAPI.h"
#include "FCGeometryTree.h"
#include "FCGeometryNodeExecutor.h"
#include "FCSelectionRule.h"
#include <QHash>
#include <QObject>
#include <QSet>
#include <QVariant>

namespace FC {

/**
 * @brief 几何构建引擎
 * 对应设计文档：build() = topoSort → 按序 applyNode → nodeResults；修改时 markDirty + 只重算 dirty
 */
class FCGEOMETRYENTITY_API FCGeometryBuildEngine : public QObject
{
    Q_OBJECT
public:
    explicit FCGeometryBuildEngine(QObject* parent = nullptr);
    ~FCGeometryBuildEngine() override;

    void setTree(FCGeometryTree* tree);
    FCGeometryTree* tree() const { return m_tree; }
    /** 设置节点执行器（OCC 层注入） */
    void setExecutor(FCGeometryNodeExecutor* executor);
    FCGeometryNodeExecutor* executor() const { return m_executor; }

    /** 为需要规则的操作（如 Fillet）绑定 Selection 规则 */
    void setSelectionRuleForNode(int nodeId, FCSelectionRule* rule);
    FCSelectionRule* selectionRuleForNode(int nodeId) const;

    /**
     * @brief 构建：按拓扑序执行节点，更新 nodeResults
     * @return 末节点（输出）的形状；若无节点或失败返回无效 QVariant
     */
    QVariant build();

    /** 仅重算 dirty 节点及其下游（局部重建） */
    QVariant buildDirty();

    /** 某节点结果（缓存） */
    QVariant nodeResult(int nodeId) const { return m_nodeResults.value(nodeId); }
    const QHash<int, QVariant>& nodeResults() const { return m_nodeResults; }

    /** 标记节点及其所有下游为 dirty（修改中间步骤时调用） */
    void markDirty(int nodeId);
    /** 当前 dirty 节点集合 */
    QSet<int> dirtyNodes() const { return m_dirtyNodes; }
    void clearDirty() { m_dirtyNodes.clear(); }

    /** 清除某节点及其下游的缓存（与 markDirty 一致） */
    void invalidateDownstream(int nodeId);

signals:
    void buildFinished();
    void buildFailed(int nodeId);

private:
    /** 对单个节点执行并写入 m_nodeResults */
    bool applyNode(int nodeId);

    FCGeometryTree* m_tree{ nullptr };
    FCGeometryNodeExecutor* m_executor{ nullptr };
    QHash<int, QVariant> m_nodeResults;
    QSet<int> m_dirtyNodes;
    QHash<int, FCSelectionRule*> m_selectionRules;
};

} // namespace FC

#endif // FCGEOMETRYBUILDENGINE_H
