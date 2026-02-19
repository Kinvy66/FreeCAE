/**
 * @file FCGeometryModule.h
 * @brief 函数式几何模块对外 API（addBlock / addUnion / addFillet / updateNode / build）
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#ifndef FCGEOMETRYMODULE_H
#define FCGEOMETRYMODULE_H

#include "FCGeometryEntityAPI.h"
#include "FCGeoNode.h"
#include "FCGeometryTree.h"
#include "FCGeometryBuildEngine.h"
#include "FCSelectionRule.h"
#include <QObject>
#include <QScopedPointer>
#include <QVariant>

namespace FC {

/**
 * @brief 函数式几何模块
 * 对应设计文档 GeometryModule：DAG + 重建引擎 + 规则选择，仅保存操作与依赖
 */
class FCGEOMETRYENTITY_API FCGeometryModule : public QObject
{
    Q_OBJECT
public:
    explicit FCGeometryModule(QObject* parent = nullptr);
    ~FCGeometryModule() override;

    /** 设置节点执行器（OCC 等内核实现） */
    void setExecutor(FCGeometryNodeExecutor* executor);

    /** 添加 Block 节点，返回节点 ID */
    int addBlock(const FCGeoParamSet& params, const QString& name = QString());
    /** 添加 Cylinder 节点 */
    int addCylinder(const FCGeoParamSet& params, const QString& name = QString());
    /** 添加 Sphere 节点 */
    int addSphere(const FCGeoParamSet& params, const QString& name = QString());
    /** 添加 Union 节点，inputs = [a, b] */
    int addUnion(int a, int b, const QString& name = QString());
    /** 添加 Difference 节点 */
    int addDifference(int a, int b, const QString& name = QString());
    /** 添加 Intersection 节点 */
    int addIntersection(int a, int b, const QString& name = QString());
    /** 添加 Fillet 节点，input 为要倒角的形状节点，rule 为选边/选面规则 */
    int addFillet(int input, FCSelectionRule* rule, const FCGeoParamSet& params = FCGeoParamSet(), const QString& name = QString());
    /** 添加 Import 节点（如从文件） */
    int addImport(const FCGeoParamSet& params, const QString& name = QString());

    /** 更新节点参数并标记 dirty，触发后续 buildDirty 时重算 */
    void updateNode(int id, const FCGeoParamSet& params);

    /** 全量构建，返回最终形状 */
    QVariant build();
    /** 仅重算 dirty 节点（修改中间步骤后调用） */
    QVariant buildDirty();

    FCGeometryTree* tree() const { return m_tree.get(); }
    FCGeometryBuildEngine* engine() const { return m_engine.get(); }

    /** 获取末节点 ID（build 的输出节点），用于多输出时扩展 */
    int lastOutputNodeId() const;

private:
    int appendNode(FCGeoOpType type, const QList<int>& inputs, const FCGeoParamSet& params, const QString& name);

    QScopedPointer<FCGeometryTree> m_tree;
    QScopedPointer<FCGeometryBuildEngine> m_engine;
    int m_lastNodeId{ -1 };
};

} // namespace FC

#endif // FCGEOMETRYMODULE_H
