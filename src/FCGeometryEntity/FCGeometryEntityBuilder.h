/**
 * @file FCGeometryEntityBuilder.h
 * @brief 从 FCGeoCommandList / FCGlobalGeoComponentManager 构建几何实体模型
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#ifndef FCGEOMETRYENTITYBUILDER_H
#define FCGEOMETRYENTITYBUILDER_H

#include "FCGeometryEntityAPI.h"
#include "FCGeometryEntityModel.h"
#include <QObject>

namespace FC {

class FCGeoCommandList;
class FCGlobalGeoComponentManager;
class FCGlobalGeoComponent;
class FCGeoComponent;

/**
 * @brief 几何实体构建器
 * 从几何命令列表的全局几何分组生成 Domain/Boundary/Edge/Point 实体视图
 */
class FCGEOMETRYENTITY_API FCGeometryEntityBuilder : public QObject
{
    Q_OBJECT
public:
    explicit FCGeometryEntityBuilder(QObject* parent = nullptr);
    ~FCGeometryEntityBuilder() override;

    /**
     * @brief 从几何命令列表构建实体模型
     * @param geoCommandList 当前几何命令列表（可为 nullptr）
     * @return 新建的 FCGeometryEntityModel，调用方负责释放；失败返回 nullptr
     */
    FCGeometryEntityModel* build(FCGeoCommandList* geoCommandList);

    /**
     * @brief 从全局几何分组管理器构建实体模型（供无命令列表时使用）
     * @param compMgr 全局几何分组管理器，可为 nullptr
     * @return 新建的 FCGeometryEntityModel，调用方负责释放；失败返回 nullptr
     */
    FCGeometryEntityModel* buildFromGlobalGeoComponentManager(FCGlobalGeoComponentManager* compMgr);

    /**
     * @brief 从几何命令列表重建已有实体模型（清空后按当前几何重新填充）
     * @param geoCommandList 当前几何命令列表
     * @param existingModel 已存在的几何实体模型，将被清空并重新填充
     * @return 成功返回 true
     */
    bool rebuild(FCGeoCommandList* geoCommandList, FCGeometryEntityModel* existingModel);

    /**
     * @brief 从全局几何分组管理器重建已有实体模型（供 DAG 几何等使用）
     * @param compMgr 全局几何分组管理器
     * @param existingModel 已存在的几何实体模型，将被清空并重新填充
     * @return 成功返回 true
     */
    bool rebuild(FCGlobalGeoComponentManager* compMgr, FCGeometryEntityModel* existingModel);

private:
    /**
     * @brief 从全局几何组件填充实体 ID 与 member 映射到模型
     * @param globalComp 全局几何组件
     * @param level 实体层级（Domain/Boundary/Edge/Point）
     * @param entityIds 当前层级已收集的实体 ID 列表（会追加）
     * @param model 目标几何实体模型
     */
    void fillFromComponent(FCGlobalGeoComponent* globalComp,
                           FCGeometryEntityLevel level,
                           QList<int>* entityIds,
                           FCGeometryEntityModel* model);
};

} // namespace FC

#endif // FCGEOMETRYENTITYBUILDER_H
