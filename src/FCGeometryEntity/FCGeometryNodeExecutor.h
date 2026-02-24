/**
 * @file FCGeometryNodeExecutor.h
 * @brief 几何节点执行器接口（可扩展到 OCC 等内核）
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#ifndef FCGEOMETRYNODEEXECUTOR_H
#define FCGEOMETRYNODEEXECUTOR_H

#include "FCGeometryEntityAPI.h"
#include "FCGeoNode.h"
#include "FCSelectionRule.h"
#include <QHash>
#include <QVariant>

namespace FC {

/**
 * @brief 几何节点执行器抽象接口
 * 对应设计文档 applyNode：根据节点类型与输入结果执行几何操作，返回输出形状
 * OCC 内核在 FCGeometryCommand 中实现此类，将 nodeResults 中的 QVariant 解释为 TopoDS_Shape
 */
class FCGEOMETRYENTITY_API FCGeometryNodeExecutor
{
public:
    virtual ~FCGeometryNodeExecutor() = default;

    /**
     * @brief 执行单个节点
     * @param node 当前节点
     * @param nodeResults 已计算的输入节点结果（id -> shape 的 QVariant）
     * @param selectionRule 若该节点需要规则（如 Fillet），由引擎传入
     * @return 该节点的输出形状（QVariant，OCC 为 TopoDS_Shape）
     */
    virtual QVariant execute(const FCGeoNode& node,
                             const QHash<int, QVariant>& nodeResults,
                             FCSelectionRule* selectionRule = nullptr) const = 0;

    /**
     * @brief 该执行器是否支持指定操作类型
     * @param type 几何操作类型
     * @return 支持返回 true，否则 false
     */
    virtual bool supports(FCGeoOpType type) const = 0;
};

} // namespace FC

#endif // FCGEOMETRYNODEEXECUTOR_H
