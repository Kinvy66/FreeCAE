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
#include <FCData/FCType.h>
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

    /**
     * @brief 设置几何 DAG 树
     * @param tree 几何树，可为 nullptr
     */
    void setTree(FCGeometryTree* tree);

    /** @brief 获取几何 DAG 树 */
    FCGeometryTree* tree() const { return m_tree; }

    /**
     * @brief 设置节点执行器（OCC 层注入）
     * @param executor 节点执行器
     */
    void setExecutor(FCGeometryNodeExecutor* executor);

    /** @brief 获取当前节点执行器 */
    FCGeometryNodeExecutor* executor() const { return m_executor; }

    /**
     * @brief 为需要规则的操作（如 Fillet）绑定 Selection 规则
     * @param nodeId 节点 ID
     * @param rule 选择规则，nullptr 表示移除
     */
    void setSelectionRuleForNode(FCID nodeId, FCSelectionRule* rule);

    /**
     * @brief 获取节点绑定的选择规则
     * @param nodeId 节点 ID
     * @return 规则指针，未绑定为 nullptr
     */
    FCSelectionRule* selectionRuleForNode(FCID nodeId) const;

    /**
     * @brief 构建：按拓扑序执行节点，更新 nodeResults
     * @return 末节点（输出）的形状；若无节点或失败返回无效 QVariant
     */
    QVariant build();

    /**
     * @brief 仅重算 dirty 节点及其下游（局部重建）
     * @return 末节点输出形状；无节点或失败为无效 QVariant
     */
    QVariant buildDirty();

    /**
     * @brief 获取某节点缓存结果
     * @param nodeId 节点 ID
     * @return 该节点的输出形状（QVariant），未计算为无效 QVariant
     */
    QVariant nodeResult(FCID nodeId) const { return m_nodeResults.value(nodeId); }

    /** @brief 所有节点 ID 到结果的缓存 */
    const QHash<FCID, QVariant>& nodeResults() const { return m_nodeResults; }

    /**
     * @brief 标记节点及其所有下游为 dirty（修改中间步骤时调用）
     * @param nodeId 被修改的节点 ID
     */
    void markDirty(FCID nodeId);

    /** @brief 当前 dirty 节点集合 */
    QSet<FCID> dirtyNodes() const { return m_dirtyNodes; }

    /** @brief 清空 dirty 集合 */
    void clearDirty() { m_dirtyNodes.clear(); }

    /**
     * @brief 清除某节点及其下游的缓存（与 markDirty 一致）
     * @param nodeId 节点 ID
     */
    void invalidateDownstream(FCID nodeId);

signals:
    /** @brief 构建成功完成时发射 */
    void buildFinished();

    /**
     * @brief 构建失败时发射
     * @param nodeId 失败的节点 ID
     */
    void buildFailed(FCID nodeId);

private:
    bool applyNode(FCID nodeId);

    FCGeometryTree* m_tree{ nullptr };
    FCGeometryNodeExecutor* m_executor{ nullptr };
    QHash<FCID, QVariant> m_nodeResults;
    QSet<FCID> m_dirtyNodes;
    QHash<FCID, FCSelectionRule*> m_selectionRules;
};

} // namespace FC

#endif // FCGEOMETRYBUILDENGINE_H
