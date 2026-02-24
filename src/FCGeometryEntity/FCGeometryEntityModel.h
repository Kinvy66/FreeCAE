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
    /**
     * @brief 构造几何实体模型
     * @param parentDataID 父数据 ID，默认 -1
     */
    explicit FCGeometryEntityModel(int parentDataID = -1);

    /** @brief 析构 */
    ~FCGeometryEntityModel() override;

    // ---------- 实体 ID 列表 ----------
    /** @brief 获取 Domain（体/面/线）ID 列表 */
    QList<int> getDomainIds() const { return m_domainIds; }
    /** @brief 获取 Boundary（面/边/点）ID 列表 */
    QList<int> getBoundaryIds() const { return m_boundaryIds; }
    /** @brief 获取 Edge（边/点）ID 列表 */
    QList<int> getEdgeIds() const { return m_edgeIds; }
    /** @brief 获取 Point（点）ID 列表 */
    QList<int> getPointIds() const { return m_pointIds; }

    /** @brief 设置 Domain ID 列表 */
    void setDomainIds(const QList<int>& ids) { m_domainIds = ids; }
    /** @brief 设置 Boundary ID 列表 */
    void setBoundaryIds(const QList<int>& ids) { m_boundaryIds = ids; }
    /** @brief 设置 Edge ID 列表 */
    void setEdgeIds(const QList<int>& ids) { m_edgeIds = ids; }
    /** @brief 设置 Point ID 列表 */
    void setPointIds(const QList<int>& ids) { m_pointIds = ids; }

    // ---------- 层级关系（可选，后续从 OCC 拓扑填充）----------
    /**
     * @brief 某 Domain 包含的 Boundary ID 列表
     * @param domainId Domain 实体 ID
     * @return Boundary ID 列表
     */
    QList<int> getBoundariesOfDomain(int domainId) const;

    /**
     * @brief 设置某 Domain 包含的 Boundary 列表
     * @param domainId Domain 实体 ID
     * @param boundaryIds Boundary ID 列表
     */
    void setBoundariesOfDomain(int domainId, const QList<int>& boundaryIds);

    /**
     * @brief 某 Boundary 包含的 Edge ID 列表
     * @param boundaryId Boundary 实体 ID
     * @return Edge ID 列表
     */
    QList<int> getEdgesOfBoundary(int boundaryId) const;

    /**
     * @brief 设置某 Boundary 包含的 Edge 列表
     * @param boundaryId Boundary 实体 ID
     * @param edgeIds Edge ID 列表
     */
    void setEdgesOfBoundary(int boundaryId, const QList<int>& edgeIds);

    // ---------- 可选名称 ----------
    /**
     * @brief 获取实体名称
     * @param level 实体层级
     * @param id 实体 ID
     * @return 名称，未设置则为空字符串
     */
    QString getEntityName(FCGeometryEntityLevel level, int id) const;

    /**
     * @brief 设置实体名称
     * @param level 实体层级
     * @param id 实体 ID
     * @param name 名称
     */
    void setEntityName(FCGeometryEntityLevel level, int id, const QString& name);

    // ---------- 与原始几何 member 的映射（FCGlobalGeoComponent 的 member 索引）----------
    /**
     * @brief 实体 ID 对应的原始几何 member 索引（用于与 FCGeoToMeshMapper 等衔接）
     * @param level 实体层级
     * @param entityId 实体 ID
     * @return 源 member 索引，未映射为 -1
     */
    int getSourceMemberId(FCGeometryEntityLevel level, int entityId) const;

    /**
     * @brief 设置实体到源 member 的映射
     * @param level 实体层级
     * @param entityId 实体 ID
     * @param sourceMemberId 源 member 索引
     */
    void setSourceMemberId(FCGeometryEntityLevel level, int entityId, int sourceMemberId);

    /** @brief 清空所有实体 ID、层级关系与映射 */
    void clear();

    /**
     * @brief 序列化为字符串
     * @param label 可选标签
     * @return 序列化字符串
     */
    QString serialize(int label = -1) override;

    /**
     * @brief 从字符串反序列化
     * @param text 序列化字符串
     * @param label 可选标签
     * @return 成功 true
     */
    bool deserialize(const QString& text, int label = -1) override;

    /**
     * @brief 从另一数据对象复制
     * @param obj 源对象，应为 FCGeometryEntityModel*
     * @return 成功 true
     */
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
