/**
 * @file FCGeometryEntityModel.cpp
 * @brief 几何实体模型实现
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#include "FCGeometryEntityModel.h"
#include <QStringList>

namespace FC {

static QString keyForEntity(FCGeometryEntityLevel level, int id)
{
    return QString("%1_%2").arg(static_cast<int>(level)).arg(id);
}

FCGeometryEntityModel::FCGeometryEntityModel(int parentDataID)
    : FCAbstractDataObject(parentDataID)
{
}

FCGeometryEntityModel::~FCGeometryEntityModel() = default;

QList<int> FCGeometryEntityModel::getBoundariesOfDomain(int domainId) const
{
    return m_domainToBoundaries.value(domainId);
}

void FCGeometryEntityModel::setBoundariesOfDomain(int domainId, const QList<int>& boundaryIds)
{
    m_domainToBoundaries.insert(domainId, boundaryIds);
}

QList<int> FCGeometryEntityModel::getEdgesOfBoundary(int boundaryId) const
{
    return m_boundaryToEdges.value(boundaryId);
}

void FCGeometryEntityModel::setEdgesOfBoundary(int boundaryId, const QList<int>& edgeIds)
{
    m_boundaryToEdges.insert(boundaryId, edgeIds);
}

QString FCGeometryEntityModel::getEntityName(FCGeometryEntityLevel level, int id) const
{
    return m_entityNames.value(keyForEntity(level, id));
}

void FCGeometryEntityModel::setEntityName(FCGeometryEntityLevel level, int id, const QString& name)
{
    m_entityNames.insert(keyForEntity(level, id), name);
}

int FCGeometryEntityModel::getSourceMemberId(FCGeometryEntityLevel level, int entityId) const
{
    return m_entityToSourceMember.value(keyForEntity(level, entityId), -1);
}

void FCGeometryEntityModel::setSourceMemberId(FCGeometryEntityLevel level, int entityId, int sourceMemberId)
{
    m_entityToSourceMember.insert(keyForEntity(level, entityId), sourceMemberId);
}

void FCGeometryEntityModel::clear()
{
    m_domainIds.clear();
    m_boundaryIds.clear();
    m_edgeIds.clear();
    m_pointIds.clear();
    m_domainToBoundaries.clear();
    m_boundaryToEdges.clear();
    m_entityNames.clear();
    m_entityToSourceMember.clear();
}

QString FCGeometryEntityModel::serialize(int /*label*/)
{
    QStringList out;
    QStringList d;
    for (int id : m_domainIds) d << QString::number(id);
    out << QStringLiteral("D:") + d.join(QLatin1Char(','));
    QStringList b;
    for (int id : m_boundaryIds) b << QString::number(id);
    out << QStringLiteral("B:") + b.join(QLatin1Char(','));
    QStringList e;
    for (int id : m_edgeIds) e << QString::number(id);
    out << QStringLiteral("E:") + e.join(QLatin1Char(','));
    QStringList p;
    for (int id : m_pointIds) p << QString::number(id);
    out << QStringLiteral("P:") + p.join(QLatin1Char(','));
    return out.join(QStringLiteral("\n"));
}

bool FCGeometryEntityModel::deserialize(const QString& text, int /*label*/)
{
    if (text.isEmpty()) return true;
    clear();
    QStringList lines = text.split(QLatin1Char('\n'), Qt::SkipEmptyParts);
    for (const QString& line : lines) {
        if (line.startsWith(QStringLiteral("D:"))) {
            QStringList parts = line.mid(2).split(QLatin1Char(','));
            for (const QString& p : parts) {
                bool ok;
                int id = p.trimmed().toInt(&ok);
                if (ok) m_domainIds.append(id);
            }
        } else if (line.startsWith(QStringLiteral("B:"))) {
            QStringList parts = line.mid(2).split(QLatin1Char(','));
            for (const QString& p : parts) {
                bool ok;
                int id = p.trimmed().toInt(&ok);
                if (ok) m_boundaryIds.append(id);
            }
        } else if (line.startsWith(QStringLiteral("E:"))) {
            QStringList parts = line.mid(2).split(QLatin1Char(','));
            for (const QString& p : parts) {
                bool ok;
                int id = p.trimmed().toInt(&ok);
                if (ok) m_edgeIds.append(id);
            }
        } else if (line.startsWith(QStringLiteral("P:"))) {
            QStringList parts = line.mid(2).split(QLatin1Char(','));
            for (const QString& p : parts) {
                bool ok;
                int id = p.trimmed().toInt(&ok);
                if (ok) m_pointIds.append(id);
            }
        }
    }
    return true;
}

bool FCGeometryEntityModel::copy(FCAbstractDataObject* obj)
{
    FCGeometryEntityModel* other = dynamic_cast<FCGeometryEntityModel*>(obj);
    if (!other) return FCAbstractDataObject::copy(obj);
    m_domainIds = other->m_domainIds;
    m_boundaryIds = other->m_boundaryIds;
    m_edgeIds = other->m_edgeIds;
    m_pointIds = other->m_pointIds;
    m_domainToBoundaries = other->m_domainToBoundaries;
    m_boundaryToEdges = other->m_boundaryToEdges;
    m_entityNames = other->m_entityNames;
    m_entityToSourceMember = other->m_entityToSourceMember;
    return FCAbstractDataObject::copy(obj);
}

} // namespace FC
