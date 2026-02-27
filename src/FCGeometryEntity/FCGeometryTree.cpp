/**
 * @file FCGeometryTree.cpp
 * @brief 几何 DAG 管理器实现
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#include "FCGeometryTree.h"
#include <QRegularExpression>

namespace FC {

void FCGeometryTree::setNextNodeId(FCID)
{
    // 节点 ID 现由 FCDataRepo / FCUniqueIDGenerater 分配，此处保留接口兼容
}

FCID FCGeometryTree::addNode(const FCGeoNode& node)
{
    if (node.id == FCID_INVALID || m_nodes.contains(node.id))
        return node.id;
    m_nodes.insert(node.id, node);
    for (FCID inId : node.inputs)
        m_downstream[inId].append(node.id);
    return node.id;
}

void FCGeometryTree::removeNode(FCID id)
{
    FCGeoNode n = m_nodes.value(id);
    m_nodes.remove(id);
    for (FCID inId : n.inputs) {
        QList<FCID>& list = m_downstream[inId];
        list.removeAll(id);
        if (list.isEmpty()) m_downstream.remove(inId);
    }
    m_downstream.remove(id);
}

void FCGeometryTree::setNode(FCID id, const FCGeoNode& node)
{
    if (!m_nodes.contains(id)) return;
    FCGeoNode old = m_nodes.value(id);
    m_nodes.insert(id, node);
    for (FCID inId : old.inputs) {
        m_downstream[inId].removeAll(id);
        if (m_downstream[inId].isEmpty()) m_downstream.remove(inId);
    }
    for (FCID inId : node.inputs)
        m_downstream[inId].append(id);
}

QList<FCID> FCGeometryTree::topoSort() const
{
    QList<FCID> order;
    QSet<FCID> visited, temp;
    for (FCID id : m_nodes.keys()) {
        if (visited.contains(id)) continue;
        if (!topoSortVisit(id, visited, temp, order))
            return QList<FCID>();
    }
    return order;
}

bool FCGeometryTree::topoSortVisit(FCID id, QSet<FCID>& visited, QSet<FCID>& temp, QList<FCID>& order) const
{
    if (temp.contains(id)) return false;
    if (visited.contains(id)) return true;
    temp.insert(id);
    FCGeoNode n = m_nodes.value(id);
    for (FCID inId : n.inputs) {
        if (m_nodes.contains(inId) && !topoSortVisit(inId, visited, temp, order))
            return false;
    }
    temp.remove(id);
    visited.insert(id);
    order.append(id);
    return true;
}

QList<FCID> FCGeometryTree::getDownstream(FCID id) const
{
    return m_downstream.value(id);
}

QSet<FCID> FCGeometryTree::getDownstreamRecursive(FCID id) const
{
    QSet<FCID> result;
    result.insert(id);
    QList<FCID> stack = m_downstream.value(id);
    while (!stack.isEmpty()) {
        FCID cur = stack.takeFirst();
        if (result.contains(cur)) continue;
        result.insert(cur);
        for (FCID d : m_downstream.value(cur)) {
            if (!result.contains(d)) stack.append(d);
        }
    }
    return result;
}

void FCGeometryTree::clear()
{
    m_nodes.clear();
    m_downstream.clear();
}

QString FCGeometryTree::checkName(const QString& name) const
{
    // 与 FCAbstractDataManagerHelper 一致：支持 "前缀_数字" 或 "前缀-数字"，取同前缀最大编号+1
    QString simplified = name.simplified();
    if (simplified.isEmpty())
        return QStringLiteral("Node_1");

    QRegularExpression suffixRegex(QStringLiteral("^(.+)[_\\-](\\d+)$"));
    QRegularExpressionMatch m = suffixRegex.match(simplified);
    QString prefix;
    int suggestedNum = 1;
    if (m.hasMatch()) {
        prefix = m.captured(1);
        suggestedNum = m.captured(2).toInt();
    } else {
        prefix = simplified;
    }

    int maxNum = 0;
    QRegularExpression prefixNumRegex(QStringLiteral("^(.+)[_\\-](\\d+)$"));
    for (const FCGeoNode& n : m_nodes) {
        QRegularExpressionMatch om = prefixNumRegex.match(n.name);
        if (!om.hasMatch()) continue;
        if (om.captured(1).compare(prefix, Qt::CaseInsensitive) != 0) continue;
        int num = om.captured(2).toInt();
        if (num > maxNum) maxNum = num;
    }
    int nextNum = (maxNum >= suggestedNum) ? (maxNum + 1) : suggestedNum;
    QString candidate = prefix + QLatin1Char('_') + QString::number(nextNum);
    for (;;) {
        bool taken = false;
        for (const FCGeoNode& n : m_nodes) {
            if (n.name.compare(candidate, Qt::CaseInsensitive) == 0) {
                taken = true;
                break;
            }
        }
        if (!taken) return candidate;
        candidate = prefix + QLatin1Char('_') + QString::number(++nextNum);
    }
}

} // namespace FC
