/**
 * @file FCGeometryDAGData.cpp
 * @brief COMSOL 式函数式几何数据实现
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#include "FCGeometryDAGData.h"
#include "FCGeoOpType.h"
#include "FCGeometryTree.h"
#include <FCGeometryInterface/FCGlobalGeoComponent.h>
#include <algorithm>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMetaObject>

namespace FC {

static QString geoOpTypeToString(FCGeoOpType t)
{
    switch (t) {
    case FCGeoOpType::None:         return QStringLiteral("None");
    case FCGeoOpType::Block:        return QStringLiteral("Block");
    case FCGeoOpType::Cylinder:    return QStringLiteral("Cylinder");
    case FCGeoOpType::Sphere:      return QStringLiteral("Sphere");
    case FCGeoOpType::Union:       return QStringLiteral("Union");
    case FCGeoOpType::Difference:   return QStringLiteral("Difference");
    case FCGeoOpType::Intersection: return QStringLiteral("Intersection");
    case FCGeoOpType::Fillet:      return QStringLiteral("Fillet");
    case FCGeoOpType::Chamfer:     return QStringLiteral("Chamfer");
    case FCGeoOpType::Import:     return QStringLiteral("Import");
    case FCGeoOpType::Extrude:     return QStringLiteral("Extrude");
    case FCGeoOpType::Revolve:     return QStringLiteral("Revolve");
    case FCGeoOpType::Sweep:       return QStringLiteral("Sweep");
    }
    return QStringLiteral("None");
}

static FCGeoOpType stringToGeoOpType(const QString& s)
{
    if (s == QLatin1String("Block"))        return FCGeoOpType::Block;
    if (s == QLatin1String("Cylinder"))     return FCGeoOpType::Cylinder;
    if (s == QLatin1String("Sphere"))      return FCGeoOpType::Sphere;
    if (s == QLatin1String("Union"))       return FCGeoOpType::Union;
    if (s == QLatin1String("Difference"))   return FCGeoOpType::Difference;
    if (s == QLatin1String("Intersection")) return FCGeoOpType::Intersection;
    if (s == QLatin1String("Fillet"))      return FCGeoOpType::Fillet;
    if (s == QLatin1String("Chamfer"))     return FCGeoOpType::Chamfer;
    if (s == QLatin1String("Import"))      return FCGeoOpType::Import;
    if (s == QLatin1String("Extrude"))     return FCGeoOpType::Extrude;
    if (s == QLatin1String("Revolve"))     return FCGeoOpType::Revolve;
    if (s == QLatin1String("Sweep"))       return FCGeoOpType::Sweep;
    return FCGeoOpType::None;
}

FCGeometryDAGData::FCGeometryDAGData()
{
    m_module.reset(new FCGeometryModule(this));
    m_compMgr.reset(new FCGlobalGeoComponentManager);
    connect(m_module->engine(), &FCGeometryBuildEngine::buildFinished,
            this, &FCGeometryDAGData::onBuildFinished);
}

FCGeometryDAGData::~FCGeometryDAGData() = default;

void FCGeometryDAGData::setDirty(bool on)
{
    if (m_dirty == on) return;
    m_dirty = on;
    emit dirtyStateChanged(m_dirty);
}

void FCGeometryDAGData::setExecutor(FCGeometryNodeExecutor* executor)
{
    if (m_module)
        m_module->setExecutor(executor);
}

void FCGeometryDAGData::ensureBuild()
{
    if (!m_module || !m_module->engine() || !m_module->engine()->executor())
        return;
    if (m_module->tree() && m_module->tree()->nodeCount() > 0)
        m_module->build();
}

void FCGeometryDAGData::onBuildFinished()
{
    if (!m_compMgr || !m_buildResultFiller)
        return;
    QVariant result = m_module->engine()->nodeResult(m_module->lastOutputNodeId());
    if (result.isValid())
        m_buildResultFiller(result, m_compMgr.get());
}

QList<FCGeoNode> FCGeometryDAGData::getGeometrySequence() const
{
    QList<FCGeoNode> out;
    if (!m_module || !m_module->tree())
        return out;
    const FCGeometryTree* tree = m_module->tree();
    QList<int> ids = tree->nodeIds();
    std::sort(ids.begin(), ids.end());
    for (int id : ids)
        out.append(tree->node(id));
    return out;
}

QString FCGeometryDAGData::serialize(int /*label*/)
{
    if (!m_module || !m_module->tree())
        return QString();
    const FCGeometryTree* tree = m_module->tree();
    QJsonArray arr;
    for (int id : tree->nodeIds()) {
        FCGeoNode n = tree->node(id);
        QJsonObject obj;
        obj[QStringLiteral("id")] = n.id;
        obj[QStringLiteral("type")] = geoOpTypeToString(n.type);
        obj[QStringLiteral("name")] = n.name;
        QJsonObject paramsObj;
        for (auto it = n.params.begin(); it != n.params.end(); ++it) {
            QVariant v = it.value();
            if (v.type() == QVariant::Double)
                paramsObj[it.key()] = v.toDouble();
            else if (v.type() == QVariant::Int)
                paramsObj[it.key()] = v.toInt();
            else
                paramsObj[it.key()] = v.toString();
        }
        obj[QStringLiteral("params")] = paramsObj;
        QJsonArray inputsArr;
        for (int inId : n.inputs)
            inputsArr.append(inId);
        obj[QStringLiteral("inputs")] = inputsArr;
        arr.append(obj);
    }
    QJsonObject root;
    root[QStringLiteral("version")] = 1;
    root[QStringLiteral("nodes")] = arr;
    return QString::fromUtf8(QJsonDocument(root).toJson(QJsonDocument::Compact));
}

bool FCGeometryDAGData::deserialize(const QString& text, int /*label*/)
{
    if (!m_module || !m_module->tree() || text.isEmpty())
        return true;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(text.toUtf8(), &err);
    if (doc.isNull() || !doc.isObject())
        return false;
    QJsonObject root = doc.object();
    if (root[QStringLiteral("version")].toInt(0) != 1)
        return false;
    QJsonArray arr = root[QStringLiteral("nodes")].toArray();
    if (arr.isEmpty()) {
        m_module->tree()->clear();
        return true;
    }
    FCGeometryTree* tree = m_module->tree();
    tree->clear();
    int maxId = 0;
    for (int i = 0; i < arr.size(); ++i) {
        QJsonObject obj = arr.at(i).toObject();
        FCGeoNode node;
        node.id = obj[QStringLiteral("id")].toInt(-1);
        node.type = stringToGeoOpType(obj[QStringLiteral("type")].toString());
        node.name = obj[QStringLiteral("name")].toString();
        QJsonObject paramsObj = obj[QStringLiteral("params")].toObject();
        for (auto it = paramsObj.begin(); it != paramsObj.end(); ++it) {
            if (it.value().isDouble())
                node.params[it.key()] = it.value().toDouble();
            else if (it.value().isBool())
                node.params[it.key()] = it.value().toBool();
            else
                node.params[it.key()] = it.value().toString();
        }
        QJsonArray inputsArr = obj[QStringLiteral("inputs")].toArray();
        for (int j = 0; j < inputsArr.size(); ++j)
            node.inputs.append(inputsArr.at(j).toInt());
        if (node.id >= 0) {
            tree->addNode(node);
            if (node.id > maxId)
                maxId = node.id;
        }
    }
    tree->setNextNodeId(maxId);
    m_dirty = false;
    return true;
}

bool FCGeometryDAGData::copy(FCAbstractDataObject* obj)
{
    FCGeometryDAGData* other = dynamic_cast<FCGeometryDAGData*>(obj);
    if (!other)
        return FCAbstractDataObject::copy(obj);
    // 浅拷贝：仅拷贝数据对象基类信息；DAG 内容可后续扩展
    return FCAbstractDataObject::copy(obj);
}

} // namespace FC
