/**
 * @file FCGeometryEntityModel.h
 * @brief 几何实体模型（一次 Build 后的 Domain/Boundary/Edge/Point 视图）
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#ifndef FCGEOMETRYENTITYMODEL_H
#define FCGEOMETRYENTITYMODEL_H

#include "FCGeometryEntityAPI.h"
#include "FCGeometryEntityLevel.h"
#include <FCData/FCAbstractDataObject.h>
#include <FCData/FCMacros.h>
#include <QHash>
#include <QList>
#include <QString>

namespace FC {

/**
 * @brief 几何实体模型
 * 表示一次 Build 后的几何实体视图，提供稳定的 Domain/Boundary/Edge/Point ID 及选择接口
 */
class FCGEOMETRYENTITY_API FCGeometryEntityModel : public FCAbstractDataObject
{
    FC_CLASS(FC, FCGeometryEntityModel);
public:
    explicit FCGeometryEntityModel(int parentDataID = -1);
    ~FCGeometryEntityModel() override;

    // ---------- 实体 ID 列表 ----------
    QList<int> getDomainIds() const { return m_domainIds; }
    QList<int> getBoundaryIds() const { return m_boundaryIds; }
    QList<int> getEdgeIds() const { return m_edgeIds; }
    QList<int> getPointIds() const { return m_pointIds; }

    void setDomainIds(const QList<int>& ids) { m_domainIds = ids; }
    void setBoundaryIds(const QList<int>& ids) { m_boundaryIds = ids; }
    void setEdgeIds(const QList<int>& ids) { m_edgeIds = ids; }
    void setPointIds(const QList<int>& ids) { m_pointIds = ids; }

    // ---------- 层级关系（可选，后续从 OCC 拓扑填充）----------
    /** 某 Domain 包含的 Boundary ID 列表 */
    QList<int> getBoundariesOfDomain(int domainId) const;
    void setBoundariesOfDomain(int domainId, const QList<int>& boundaryIds);
    /** 某 Boundary 包含的 Edge ID 列表 */
    QList<int> getEdgesOfBoundary(int boundaryId) const;
    void setEdgesOfBoundary(int boundaryId, const QList<int>& edgeIds);

    // ---------- 可选名称 ----------
    QString getEntityName(FCGeometryEntityLevel level, int id) const;
    void setEntityName(FCGeometryEntityLevel level, int id, const QString& name);

    // ---------- 与原始几何 member 的映射（FCGlobalGeoComponent 的 member 索引）----------
    /** 实体 ID 对应的原始几何 member 索引（用于与 FCGeoToMeshMapper 等衔接）*/
    int getSourceMemberId(FCGeometryEntityLevel level, int entityId) const;
    void setSourceMemberId(FCGeometryEntityLevel level, int entityId, int sourceMemberId);

    void clear();

    QString serialize(int label = -1) override;
    bool deserialize(const QString& text, int label = -1) override;
    bool copy(FCAbstractDataObject* obj) override;

private:
    QList<int> m_domainIds;
    QList<int> m_boundaryIds;
    QList<int> m_edgeIds;
    QList<int> m_pointIds;
    QHash<int, QList<int>> m_domainToBoundaries;
    QHash<int, QList<int>> m_boundaryToEdges;
    QHash<QString, QString> m_entityNames;   // key: "level_id"
    QHash<QString, int> m_entityToSourceMember; // key: "level_id" -> source member id
};

} // namespace FC

#endif // FCGEOMETRYENTITYMODEL_H
