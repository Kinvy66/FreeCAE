/**
 * @file FCOCCGeometryNodeExecutor.h
 * @brief OCC 几何节点执行器：从 FCDataRepo 按节点 ID 取命令，设置输入后 update，返回形状
 * @details 节点 ID 与几何命令 ID 一致，便于与现有数据管理适配
 */
#ifndef FCOCCGEOMETRYNODEEXECUTOR_H
#define FCOCCGEOMETRYNODEEXECUTOR_H

#include "FCGeometryCommandAPI.h"
#include <FCGeometryEntity/FCGeometryNodeExecutor.h>
#include <FCGeometryEntity/FCGeoOpType.h>

namespace OCC {

/**
 * @brief 基于 OCC 与 FCDataRepo 的几何节点执行器
 * 通过 node.id 从 FCDataRepo 获取 FCAbsGeoCommand，设置参数/输入后执行 update()，返回 TopoDS_Shape（以 QVariant 传递）
 */
class FCGEOMETRYCMD_API FCOCCGeometryNodeExecutor : public FC::FCGeometryNodeExecutor
{
public:
    FCOCCGeometryNodeExecutor() = default;
    ~FCOCCGeometryNodeExecutor() override = default;

    QVariant execute(const FC::FCGeoNode& node,
                     const QHash<int, QVariant>& nodeResults,
                     FC::FCSelectionRule* selectionRule = nullptr) const override;

    bool supports(FC::FCGeoOpType type) const override;
};

} // namespace OCC

#endif // FCOCCGEOMETRYNODEEXECUTOR_H
