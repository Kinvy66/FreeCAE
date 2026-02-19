/**
 * @file FCSelectionRule.h
 * @brief Selection 规则接口（每次重建时求值，避免固定 Face ID）
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#ifndef FCSELECTIONRULE_H
#define FCSELECTIONRULE_H

#include "FCGeometryEntityAPI.h"
#include <QList>
#include <QVariant>

namespace FC {

/**
 * @brief Selection 规则抽象接口
 * 对应设计文档 SelectionRule：在每次 build 后对当前形状求值，返回选中的子形状，避免拓扑命名问题
 * @note 形状使用 QVariant 传递，OCC 层可包装 TopoDS_Shape 并注册元类型
 */
class FCGEOMETRYENTITY_API FCSelectionRule
{
public:
    virtual ~FCSelectionRule() = default;

    /**
     * @brief 对当前形状求值，返回选中的子形状列表
     * @param shape 当前 build 得到的形状（QVariant，OCC 层为 TopoDS_Shape）
     * @return 选中的子形状列表（如边、面），用于 Fillet 等操作
     */
    virtual QList<QVariant> eval(const QVariant& shape) const = 0;
};

} // namespace FC

#endif // FCSELECTIONRULE_H
