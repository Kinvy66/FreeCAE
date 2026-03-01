/**
 * @file FCEventTypes.h
 * @brief 应用内事件类型与载荷键，供事件总线与各操作器/窗口联动使用
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 *
 * 操作器三件事：执行逻辑、修改 UI、发送事件。事件由总线统一监听与分发，
 * 任意多个 Docking 窗口均可通过订阅事件联动（如树选中、VTK 选中 domain 等）。
 */
#ifndef FCEVENTTYPES_H
#define FCEVENTTYPES_H

#include "FCInterfaceAPI.h"
#include <QVariantMap>

namespace FC {

/**
 * @brief 属性面板内容类型（与工程树节点类型对应，COMSOL 式：树选中什么就显示什么）
 * 扩展时在此增加新类型，并注册对应的 ContentHandler。
 */
enum PropertyPanelEntityType
{
    PropertyPanelEntityNone = 0,
    PropertyPanelEntityGeometry,
    PropertyPanelEntityMesh,
    PropertyPanelEntityPhysics,
    PropertyPanelEntityStudy,
    PropertyPanelEntityResult,
    PropertyPanelEntityUser = 100
};

/**
 * @brief 应用级事件类型枚举
 * 扩展时在此增加新类型，并在对应操作器/控件中 emit，在需要联动的窗口中 subscribe。
 */
enum FCAppEventType
{
    /** 工程树选中某类节点（几何/网格/物理场等）：载荷 nodeId + entityType，属性面板根据 entityType 切换内容 */
    EventTreeNodeSelected = 0,
    /** 工程树无选中：无载荷，属性面板清空 */
    EventNoEntitySelected,
    /** VTK 窗口中选中几何 domain（如布尔操作选体）：载荷 geometryIds 等 */
    EventVTKGeometryDomainSelected,
    /** 几何构建完成：可选载荷，供需要刷新的窗口订阅 */
    EventGeometryBuildCompleted,
    EventUser = 100
};

/** 事件载荷键：nodeId（树节点 ID，quint64） */
inline const char* EventPayloadKey_NodeId = "nodeId";
/** 事件载荷键：entityType（PropertyPanelEntityType，int），用于 EventTreeNodeSelected */
inline const char* EventPayloadKey_EntityType = "entityType";
/** 事件载荷键：geometryIds（VTK 选中的几何体 ID 列表） */
inline const char* EventPayloadKey_GeometryIds = "geometryIds";

} // namespace FC

#endif // FCEVENTTYPES_H
