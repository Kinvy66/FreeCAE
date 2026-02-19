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
    int id{ -1 };
    FCGeoOpType type{ FCGeoOpType::None };
    QString name;
    FCGeoParamSet params;
    /** 输入节点 ID 列表（多输入，如 Union 有两个输入） */
    QList<int> inputs;

    FCGeoNode() = default;
    FCGeoNode(int id_, FCGeoOpType type_, const QString& name_ = QString())
        : id(id_), type(type_), name(name_) {}
};

} // namespace FC

#endif // FCGEONODE_H
