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
#include <FCData/FCType.h>
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

    /**
     * @brief 设置节点执行器（OCC 等内核实现）
     * @param executor 节点执行器，由 OCC 等几何内核实现
     */
    void setExecutor(FCGeometryNodeExecutor* executor);


    /**
     * @brief 添加 Block 节点，返回节点 ID
     * @param params 节点参数集（如长宽高）
     * @param name 节点名称，为空则自动生成
     * @return 新节点 ID
     */
    FCID addBlock(const FCGeoParamSet& params, const QString& name = QString());

    /**
     * @brief 添加 Cylinder 节点
     * @param params 节点参数集（如半径、高度）
     * @param name 节点名称，为空则自动生成
     * @return 新节点 ID
     */
    FCID addCylinder(const FCGeoParamSet& params, const QString& name = QString());

    /**
     * @brief 添加 Sphere 节点
     * @param params 节点参数集（如半径）
     * @param name 节点名称，为空则自动生成
     * @return 新节点 ID
     */
    FCID addSphere(const FCGeoParamSet& params, const QString& name = QString());

    /**
     * @brief 添加 Union 节点，inputs = [a, b]
     * @param a 第一个输入节点 ID
     * @param b 第二个输入节点 ID
     * @param name 节点名称，为空则自动生成
     * @return 新节点 ID
     */
    FCID addUnion(FCID a, FCID b, const QString& name = QString());

    /**
     * @brief 添加 Difference 节点（a 减 b）
     * @param a 被减形状节点 ID
     * @param b 减去形状节点 ID
     * @param name 节点名称，为空则自动生成
     * @return 新节点 ID
     */
    FCID addDifference(FCID a, FCID b, const QString& name = QString());

    /**
     * @brief 添加 Intersection 节点
     * @param a 第一个输入节点 ID
     * @param b 第二个输入节点 ID
     * @param name 节点名称，为空则自动生成
     * @return 新节点 ID
     */
    FCID addIntersection(FCID a, FCID b, const QString& name = QString());

    /**
     * @brief 添加 Fillet 节点，input 为要倒角的形状节点，rule 为选边/选面规则
     * @param input 要倒角的形状节点 ID
     * @param rule 选边/选面规则，每次重建时求值
     * @param params 倒角参数（如半径）
     * @param name 节点名称，为空则自动生成
     * @return 新节点 ID
     */
    FCID addFillet(FCID input, FCSelectionRule* rule, const FCGeoParamSet& params = FCGeoParamSet(), const QString& name = QString());

    /**
     * @brief 添加 Import 节点（如从文件导入）
     * @param params 导入参数（如文件路径）
     * @param name 节点名称，为空则自动生成
     * @return 新节点 ID
     */
    FCID addImport(const FCGeoParamSet& params, const QString& name = QString());

    /**
     * @brief 更新节点参数并标记 dirty，触发后续 buildDirty 时重算
     * @param id 节点 ID
     * @param params 新的参数集
     */
    void updateNode(FCID id, const FCGeoParamSet& params);

    /**
     * @brief 全量构建，返回最终形状
     * @return 末节点输出形状（QVariant），无节点或失败时为无效 QVariant
     */
    QVariant build();

    /**
     * @brief 仅重算 dirty 节点（修改中间步骤后调用）
     * @return 末节点输出形状（QVariant），无节点或失败时为无效 QVariant
     */
    QVariant buildDirty();

    /** @brief 获取几何 DAG 树 */
    FCGeometryTree* tree() const { return m_tree.get(); }

    /** @brief 获取构建引擎 */
    FCGeometryBuildEngine* engine() const { return m_engine.get(); }

    /**
     * @brief 获取末节点 ID（build 的输出节点），用于多输出时扩展
     * @return 最后一次添加/构建的节点 ID，无节点时为 FCID_INVALID
     */
    FCID lastOutputNodeId() const;

    /**
     * @brief 移除节点并从 FCDataRepo 移除并销毁对应几何命令（节点 ID = 命令 ID）
     * @param id 节点/命令 ID
     */
    void removeNode(FCID id);

private:
    /**
     * @brief 内部添加节点
     * @param type 操作类型
     * @param inputs 输入节点 ID 列表
     * @param params 参数集
     * @param name 节点名称
     * @return 新节点 ID
     */
    FCID appendNode(FCGeoOpType type, const QList<FCID>& inputs, const FCGeoParamSet& params, const QString& name);

    QScopedPointer<FCGeometryTree> m_tree;
    QScopedPointer<FCGeometryBuildEngine> m_engine;
    FCID m_lastNodeId{ FCID_INVALID };
};

} // namespace FC

#endif // FCGEOMETRYMODULE_H
