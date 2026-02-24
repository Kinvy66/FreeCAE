/**
 * @file FCGeometryModule.cpp
 * @brief 函数式几何模块实现
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#include "FCGeometryModule.h"
#include <QScopedPointer>

namespace FC {

FCGeometryModule::FCGeometryModule(QObject* parent)
    : QObject(parent)
{
    m_tree.reset(new FCGeometryTree);
    m_engine.reset(new FCGeometryBuildEngine(this));
    m_engine->setTree(m_tree.get());
}

FCGeometryModule::~FCGeometryModule() = default;

void FCGeometryModule::setExecutor(FCGeometryNodeExecutor* executor)
{
    m_engine->setExecutor(executor);
}

int FCGeometryModule::appendNode(FCGeoOpType type, const QList<int>& inputs, const FCGeoParamSet& params, const QString& name)
{
    QString nodeName = name.isEmpty() ? m_tree->checkName(QStringLiteral("Node")) : m_tree->checkName(name);
    FCGeoNode node(-1, type, nodeName);
    node.inputs = inputs;
    node.params = params;
    int id = m_tree->addNode(node);
    m_lastNodeId = id;
    return id;
}

int FCGeometryModule::addBlock(const FCGeoParamSet& params, const QString& name)
{
    return appendNode(FCGeoOpType::Block, QList<int>(), params, name);
}

int FCGeometryModule::addCylinder(const FCGeoParamSet& params, const QString& name)
{
    return appendNode(FCGeoOpType::Cylinder, QList<int>(), params, name);
}

int FCGeometryModule::addSphere(const FCGeoParamSet& params, const QString& name)
{
    return appendNode(FCGeoOpType::Sphere, QList<int>(), params, name);
}

int FCGeometryModule::addUnion(int a, int b, const QString& name)
{
    return appendNode(FCGeoOpType::Union, QList<int>() << a << b, FCGeoParamSet(), name);
}

int FCGeometryModule::addDifference(int a, int b, const QString& name)
{
    return appendNode(FCGeoOpType::Difference, QList<int>() << a << b, FCGeoParamSet(), name);
}

int FCGeometryModule::addIntersection(int a, int b, const QString& name)
{
    return appendNode(FCGeoOpType::Intersection, QList<int>() << a << b, FCGeoParamSet(), name);
}

int FCGeometryModule::addFillet(int input, FCSelectionRule* rule, const FCGeoParamSet& params, const QString& name)
{
    int id = appendNode(FCGeoOpType::Fillet, QList<int>() << input, params, name);
    if (rule)
        m_engine->setSelectionRuleForNode(id, rule);
    return id;
}

int FCGeometryModule::addImport(const FCGeoParamSet& params, const QString& name)
{
    return appendNode(FCGeoOpType::Import, QList<int>(), params, name);
}

void FCGeometryModule::updateNode(int id, const FCGeoParamSet& params)
{
    if (!m_tree->hasNode(id)) return;
    FCGeoNode node = m_tree->node(id);
    node.params = params;
    m_tree->setNode(id, node);
    m_engine->markDirty(id);
    m_engine->invalidateDownstream(id);
}

QVariant FCGeometryModule::build()
{
    return m_engine->build();
}

QVariant FCGeometryModule::buildDirty()
{
    return m_engine->buildDirty();
}

int FCGeometryModule::lastOutputNodeId() const
{
    return m_lastNodeId;
}

} // namespace FC
