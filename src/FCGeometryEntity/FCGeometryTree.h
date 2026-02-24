/**
 * @file FCGeometryTree.h
 * @brief 几何 DAG 管理器（有向无环图，拓扑排序，下游查询）
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#ifndef FCGEOMETRYTREE_H
#define FCGEOMETRYTREE_H

#include "FCGeometryEntityAPI.h"
#include "FCGeoNode.h"
#include <QHash>
#include <QList>
#include <QSet>
#include <QString>

namespace FC {

/**
 * @brief 几何 DAG 管理器
 * 对应设计文档 GeometryTree：节点表、邻接关系、addNode、topoSort
 */
class FCGEOMETRYENTITY_API FCGeometryTree
{
public:
    FCGeometryTree() = default;

    /**
     * @brief 添加节点（若 id < 0 则自动分配）
     * @param node 节点数据
     * @return 节点 ID（新分配或 node.id）
     */
    int addNode(const FCGeoNode& node);

    /**
     * @brief 移除指定节点
     * @param id 节点 ID
     */
    void removeNode(int id);

    /** @brief 是否存在指定节点 */
    bool hasNode(int id) const { return m_nodes.contains(id); }

    /**
     * @brief 获取节点数据
     * @param id 节点 ID
     * @return 节点数据，不存在时返回默认构造的 FCGeoNode
     */
    FCGeoNode node(int id) const { return m_nodes.value(id); }

    /**
     * @brief 设置节点数据（需已存在）
     * @param id 节点 ID
     * @param node 新的节点数据
     */
    void setNode(int id, const FCGeoNode& node);

    /**
     * @brief 拓扑排序，返回执行顺序；若存在环则返回空列表
     * @return 按依赖顺序排列的节点 ID 列表，有环时为空列表
     */
    QList<int> topoSort() const;

    /**
     * @brief 获取依赖 id 的所有下游节点（id 作为输入的节点）
     * @param id 节点 ID
     * @return 以 id 为输入的直接下游节点 ID 列表
     */
    QList<int> getDownstream(int id) const;

    /**
     * @brief 递归获取 id 及其所有下游节点（用于 dirty 传播）
     * @param id 起始节点 ID
     * @return 包含 id 及其所有下游节点 ID 的集合
     */
    QSet<int> getDownstreamRecursive(int id) const;

    /** @brief 节点数量 */
    int nodeCount() const { return m_nodes.size(); }

    /** @brief 所有节点 ID 列表 */
    QList<int> nodeIds() const { return m_nodes.keys(); }

    /** @brief 清空所有节点与邻接关系 */
    void clear();

    /**
     * @brief 检查名称并返回不重复的名称（后缀递增，如 Box_1、Box_2）
     * @param name 期望名称，可为 "Box" 或 "Box_1"
     * @return 与现有节点名称不冲突的名称（同前缀取最大编号+1）
     */
    QString checkName(const QString& name) const;

    /**
     * @brief 设置下一节点 ID 的起始值（反序列化后调用，避免新节点 ID 与已加载冲突）
     * @param atLeastId 下一 ID 将 >= atLeastId + 1
     */
    void setNextNodeId(int atLeastId);

private:
    /**
     * @brief 拓扑排序 DFS 访问
     * @param id 当前节点 ID
     * @param visited 已访问集合
     * @param temp 当前路径集合（用于检测环）
     * @param order 输出顺序（尾部追加）
     * @return 无环为 true，有环为 false
     */
    bool topoSortVisit(int id, QSet<int>& visited, QSet<int>& temp, QList<int>& order) const;

    /** @brief 节点表：id -> FCGeoNode */
    QHash<int, FCGeoNode> m_nodes;

    /** @brief 邻接：id -> 以 id 为输入的所有节点 ID 列表 */
    QHash<int, QList<int>> m_downstream;
};

} // namespace FC

#endif // FCGEOMETRYTREE_H
