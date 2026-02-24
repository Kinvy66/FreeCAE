/**
 * @file FCGeoNode.h
 * @brief 几何 DAG 节点（仅保存操作类型、参数、依赖，不保存实体）
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#ifndef FCGEONODE_H
#define FCGEONODE_H

#include "FCGeometryEntityAPI.h"
#include "FCGeoOpType.h"
#include <QList>
#include <QMap>
#include <QString>
#include <QVariant>

namespace FC {

/**
 * @brief 参数集（操作节点参数）
 * 使用 QMap<QString, QVariant> 便于扩展与序列化
 */
using FCGeoParamSet = QMap<QString, QVariant>;

/**
 * @brief 几何 DAG 节点
 * 对应设计文档 GeoNode：仅保存操作类型、参数、依赖关系，不保存 Face ID / Solid 等实体
 */
struct FCGEOMETRYENTITY_API FCGeoNode
{
    int id{ -1 };                    /**< 节点 ID，< 0 表示未分配 */
    FCGeoOpType type{ FCGeoOpType::None };  /**< 操作类型 */
    QString name;                    /**< 节点名称 */
    FCGeoParamSet params;            /**< 参数集 */
    QList<int> inputs;               /**< 输入节点 ID 列表（多输入，如 Union 有两个输入） */

    /** @brief 默认构造 */
    FCGeoNode() = default;

    /**
     * @brief 构造节点
     * @param id_ 节点 ID
     * @param type_ 操作类型
     * @param name_ 节点名称
     */
    FCGeoNode(int id_, FCGeoOpType type_, const QString& name_ = QString())
        : id(id_), type(type_), name(name_) {}
};

} // namespace FC

#endif // FCGEONODE_H
