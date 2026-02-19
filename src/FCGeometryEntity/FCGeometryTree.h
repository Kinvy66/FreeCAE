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

namespace FC {

/**
 * @brief 几何 DAG 管理器
 * 对应设计文档 GeometryTree：节点表、邻接关系、addNode、topoSort
 */
class FCGEOMETRYENTITY_API FCGeometryTree
{
public:
    FCGeometryTree() = default;

    /** 添加节点（若 id < 0 则自动分配） */
    int addNode(const FCGeoNode& node);
    void removeNode(int id);
    bool hasNode(int id) const { return m_nodes.contains(id); }
    FCGeoNode node(int id) const { return m_nodes.value(id); }
    void setNode(int id, const FCGeoNode& node);

    /** 拓扑排序，返回执行顺序；若存在环则返回空列表 */
    QList<int> topoSort() const;

    /** 获取依赖 id 的所有下游节点（id 作为输入的节点） */
    QList<int> getDownstream(int id) const;

    /** 递归获取 id 及其所有下游节点（用于 dirty 传播） */
    QSet<int> getDownstreamRecursive(int id) const;

    int nodeCount() const { return m_nodes.size(); }
    QList<int> nodeIds() const { return m_nodes.keys(); }

    void clear();

private:
    bool topoSortVisit(int id, QSet<int>& visited, QSet<int>& temp, QList<int>& order) const;

    QHash<int, FCGeoNode> m_nodes;
    /** 邻接：id -> 以 id 为输入的所有节点 */
    QHash<int, QList<int>> m_downstream;
};

} // namespace FC

#endif // FCGEOMETRYTREE_H
