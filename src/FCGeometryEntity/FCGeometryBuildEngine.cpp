/**
 * @file FCGeometryBuildEngine.cpp
 * @brief 构建引擎实现
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#include "FCGeometryBuildEngine.h"

namespace FC {

FCGeometryBuildEngine::FCGeometryBuildEngine(QObject* parent)
    : QObject(parent)
{
}

FCGeometryBuildEngine::~FCGeometryBuildEngine() = default;

void FCGeometryBuildEngine::setTree(FCGeometryTree* tree)
{
    m_tree = tree;
    m_nodeResults.clear();
    m_dirtyNodes.clear();
}

void FCGeometryBuildEngine::setExecutor(FCGeometryNodeExecutor* executor)
{
    m_executor = executor;
}

void FCGeometryBuildEngine::setSelectionRuleForNode(FCID nodeId, FCSelectionRule* rule)
{
    if (rule)
        m_selectionRules.insert(nodeId, rule);
    else
        m_selectionRules.remove(nodeId);
}

FCSelectionRule* FCGeometryBuildEngine::selectionRuleForNode(FCID nodeId) const
{
    return m_selectionRules.value(nodeId);
}

QVariant FCGeometryBuildEngine::build()
{
    if (!m_tree || !m_executor) return QVariant();
    QList<FCID> order = m_tree->topoSort();
    if (order.isEmpty()) return QVariant();
    m_nodeResults.clear();
    m_dirtyNodes.clear();
    for (FCID id : order) {
        if (!applyNode(id)) {
            emit buildFailed(id);
            return QVariant();
        }
    }
    emit buildFinished();
    return m_nodeResults.value(order.last());
}

QVariant FCGeometryBuildEngine::buildDirty()
{
    if (!m_tree || !m_executor) return QVariant();
    if (m_dirtyNodes.isEmpty()) {
        QList<FCID> order = m_tree->topoSort();
        return order.isEmpty() ? QVariant() : m_nodeResults.value(order.last());
    }
    QList<FCID> order = m_tree->topoSort();
    if (order.isEmpty()) return QVariant();
    for (FCID id : order) {
        if (m_dirtyNodes.contains(id) && !applyNode(id)) {
            emit buildFailed(id);
            return QVariant();
        }
    }
    m_dirtyNodes.clear();
    emit buildFinished();
    return m_nodeResults.value(order.last());
}

void FCGeometryBuildEngine::markDirty(FCID nodeId)
{
    if (!m_tree) return;
    QSet<FCID> down = m_tree->getDownstreamRecursive(nodeId);
    m_dirtyNodes.unite(down);
}

void FCGeometryBuildEngine::invalidateDownstream(FCID nodeId)
{
    if (!m_tree) return;
    QSet<FCID> down = m_tree->getDownstreamRecursive(nodeId);
    for (FCID id : down)
        m_nodeResults.remove(id);
}

bool FCGeometryBuildEngine::applyNode(FCID nodeId)
{
    if (!m_tree || !m_executor) return false;
    FCGeoNode node = m_tree->node(nodeId);
    if (node.id == FCID_INVALID) return false;
    if (!m_executor->supports(node.type)) return false;
    FCSelectionRule* rule = m_selectionRules.value(nodeId);
    QVariant result = m_executor->execute(node, m_nodeResults, rule);
    if (!result.isValid()) return false;
    m_nodeResults.insert(nodeId, result);
    return true;
}

} // namespace FC
