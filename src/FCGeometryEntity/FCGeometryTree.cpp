/**
 * @file FCGeometryTree.cpp
 * @brief 几何 DAG 管理器实现
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#include "FCGeometryTree.h"
#include <QRegularExpression>

namespace FC {

static int s_nextNodeId = 1;

int FCGeometryTree::addNode(const FCGeoNode& node)
{
    FCGeoNode n = node;
    if (n.id < 0) {
        n.id = s_nextNodeId++;
    }
    if (m_nodes.contains(n.id)) {
        return n.id;
    }
    m_nodes.insert(n.id, n);
    for (int inId : n.inputs) {
        m_downstream[inId].append(n.id);
    }
    return n.id;
}

void FCGeometryTree::removeNode(int id)
{
    FCGeoNode n = m_nodes.value(id);
    m_nodes.remove(id);
    for (int inId : n.inputs) {
        QList<int>& list = m_downstream[inId];
        list.removeAll(id);
        if (list.isEmpty()) m_downstream.remove(inId);
    }
    m_downstream.remove(id);
}

void FCGeometryTree::setNode(int id, const FCGeoNode& node)
{
    if (!m_nodes.contains(id)) return;
    FCGeoNode old = m_nodes.value(id);
    m_nodes.insert(id, node);
    for (int inId : old.inputs) {
        m_downstream[inId].removeAll(id);
        if (m_downstream[inId].isEmpty()) m_downstream.remove(inId);
    }
    for (int inId : node.inputs) {
        m_downstream[inId].append(id);
    }
}

QList<int> FCGeometryTree::topoSort() const
{
    QList<int> order;
    QSet<int> visited, temp;
    for (int id : m_nodes.keys()) {
        if (visited.contains(id)) continue;
        if (!topoSortVisit(id, visited, temp, order))
            return QList<int>(); // cycle
    }
    // order 已是依赖优先（先 push 的是无依赖或依赖已 push 的），即执行顺序
    return order;
}

bool FCGeometryTree::topoSortVisit(int id, QSet<int>& visited, QSet<int>& temp, QList<int>& order) const
{
    if (temp.contains(id)) return false;
    if (visited.contains(id)) return true;
    temp.insert(id);
    FCGeoNode n = m_nodes.value(id);
    for (int inId : n.inputs) {
        if (m_nodes.contains(inId) && !topoSortVisit(inId, visited, temp, order))
            return false;
    }
    temp.remove(id);
    visited.insert(id);
    order.append(id);
    return true;
}

QList<int> FCGeometryTree::getDownstream(int id) const
{
    return m_downstream.value(id);
}

QSet<int> FCGeometryTree::getDownstreamRecursive(int id) const
{
    QSet<int> result;
    result.insert(id);
    QList<int> stack = m_downstream.value(id);
    while (!stack.isEmpty()) {
        int cur = stack.takeFirst();
        if (result.contains(cur)) continue;
        result.insert(cur);
        for (int d : m_downstream.value(cur)) {
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
