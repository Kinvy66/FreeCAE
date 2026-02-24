/**
 * @file FCGeometryDAGData.h
 * @brief COMSOL 式函数式几何数据：DAG(Directed Acyclic Graph) 定义 + 全局分组（供实体视图构建）
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#ifndef FCGEOMETRYDAGDATA_H
#define FCGEOMETRYDAGDATA_H

#include "FCGeometryEntityAPI.h"
#include "FCGeometryModule.h"
#include "FCGeometryBuildEngine.h"
#include "FCGeoNode.h"
#include <FCData/FCAbstractDataObject.h>
#include <FCData/FCMacros.h>
#include <QList>
#include <QObject>
#include <QScopedPointer>
#include <QVariant>
#include <functional>

namespace FC {

class FCGlobalGeoComponentManager;

/**
 * @brief COMSOL 式函数式几何全局数据
 *
 * 持有 FCGeometryModule（DAG）与 FCGlobalGeoComponentManager（Build 后填充，供 FCGeometryEntityBuilder 使用）。
 * 作为 GDTGeom 槽位的数据类型，替代 FCGeoCommandList 成为几何主数据源。
 * Build 结果通过 setBuildResultFiller 注入（由 OCC 层实现：将 QVariant 形状填入 compMgr）。
 */
class FCGEOMETRYENTITY_API FCGeometryDAGData : public FCAbstractDataObject
{
    Q_OBJECT
    FC_CLASS(FC, FCGeometryDAGData)
public:
    explicit FCGeometryDAGData(int parentDataID = -1);
    ~FCGeometryDAGData() override;

    /** @brief 获取 DAG 模块（addBlock/addUnion/build 等） */
    FCGeometryModule* module() const { return m_module.get(); }

    /**
     * @brief 获取全局几何分组管理器（Build 成功后由 buildResultFiller 填充）
     * @return 可能为空或未填充，调用 ensureBuild() 后再用
     */
    FCGlobalGeoComponentManager* getGlobalGeoCompManager() const { return m_compMgr.get(); }

    /**
     * @brief 设置节点执行器（OCC 等），转发到 module
     * @param executor 由 FCGeometryCommand 实现
     */
    void setExecutor(FCGeometryNodeExecutor* executor);

    /**
     * @brief 若 DAG 有节点且已设 executor，则执行 build 并触发 filler 填充 compMgr
     */
    void ensureBuild();

    /**
     * @brief 设置 Build 结果填充器：build 成功后用最终形状填充 FCGlobalGeoComponentManager
     * @param filler (shape QVariant, compMgr) -> void；OCC 层实现，遍历 TopoDS_Shape 填 Solid/Surface/Edge/Point
     */
    using BuildResultFiller = std::function<void(const QVariant& shape, FCGlobalGeoComponentManager* compMgr)>;
    void setBuildResultFiller(BuildResultFiller filler) { m_buildResultFiller = std::move(filler); }

    /**
     * @brief 几何序列：当前 DAG 中所有节点（每项 = 操作类型 FCGeoOpType + 参数 params + 依赖 inputs）
     * 对应 COMSOL 式“只保存操作与参数、不保存实体”的几何序列
     * @return 按节点 ID 顺序的节点列表（拓扑序可用 module()->tree()->topoSort()）
     */
    QList<FCGeoNode> getGeometrySequence() const;

    QString serialize(int label = -1) override;
    bool deserialize(const QString& text, int label = -1) override;
    bool copy(FCAbstractDataObject* obj) override;

private Q_SLOTS:
    void onBuildFinished();

private:
    QScopedPointer<FCGeometryModule> m_module;
    QScopedPointer<FCGlobalGeoComponentManager> m_compMgr;
    BuildResultFiller m_buildResultFiller;
};

} // namespace FC

#endif // FCGEOMETRYDAGDATA_H
