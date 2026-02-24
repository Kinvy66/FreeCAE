# FCGeometryEntity 模块与原有几何管理类的关系

本文档说明 **FCGeometryEntity** 与现有几何相关模块（FCGeometryInterface、FCGeometryCommand、FCGlobalData）的依赖关系与数据流。

---

## 一、原有几何管理相关类（上游）

| 模块 | 主要类 | 职责 |
|------|--------|------|
| **FCData** | FCGlobalData、FCGlobalDT | 全局数据容器；**GDTGeom** 槽位存几何命令列表，**GDTGeomEntity** 槽位存几何实体模型。 |
| **FCGeometryInterface** | FCGeoCommandList、FCAbsGeoCommand、FCGlobalGeoComponentManager、FCGlobalGeoComponent、FCGeoComponent | 几何**接口层**：命令列表（含撤销/重做）、基准/草图、**按类型分组的全局几何**（Solid/Surface/Edge/Point → member 列表）。 |
| **FCGeometryCommand** | 各种 OCC 命令（Box/Boolean/Extrude 等）、FCOCCGeoComponentBuilder | 几何**实现层（OCC）**：执行命令生成形状，并把 Face/Edge/Solid 等填入 **FCGlobalGeoComponentManager**（各 FCGlobalGeoComponent 内为 FCGeoComponent + member 索引）。 |
| **FCModelInterface** | FCGeoComponent、FCModelEnum::FITKModelSetType | 模型抽象：FCGeoComponent 存类型 + member 列表；FITKModelSetType 为 FMSSolid/FMSSurface/FMSEdge/FMSPoint。 |

**数据流（现有）**：

```
FCGlobalData[GDTGeom] → FCGeoCommandList
  ├─ 根命令列表 (FCAbsGeoCommand*)
  ├─ FCGlobalGeoComponentManager
  │   └─ 按类型：FCGlobalGeoComponent (Solid/Surface/Edge/Point)
  │       └─ FCGeoComponent，存 getMember() 等
  └─ 基准、草图等
```

几何“创建/编辑”由 **FCGeometryInterface + FCGeometryCommand** 完成；**FCGeoCommandList** 是当前几何的权威数据源，**FCGlobalGeoComponentManager** 是其内部按类型整理好的几何分组（由 OCC 命令在 Build 时填充）。

---

## 二、FCGeometryEntity 在整体中的位置

FCGeometryEntity **不替代**上述任何类，而是：

1. **只读消费** FCGeoCommandList / FCGlobalGeoComponentManager，在其上构建一层 **Domain/Boundary/Edge/Point 实体视图**（FCGeometryEntityModel）。
2. **可选**提供另一种建模方式（函数式 DAG：FCGeometryModule），其**执行器**由 FCGeometryCommand（OCC）实现，求值结果可由应用层写回现有几何体系。

与原有类的关系可概括为：

- **FCGeometryEntity 依赖**：FCData、FCGeometryInterface、FCModelInterface（FCGeometryEntityBuilder 依赖 FCGeoCommandList、FCGlobalGeoComponent 等；FCGeometryModule 不直接依赖 FCGeometryInterface，但需要 FCGeometryNodeExecutor 由外部注入，通常由 FCGeometryCommand 实现）。
- **原有几何不依赖 FCGeometryEntity**：FCGeoCommandList、FCGlobalGeoComponent 等不引用 FCGeometryEntity 的头文件；只有 **FCGlobalDataFactory**、**FCCubeInfoWidget** 等应用/UI 层会调用 FCGeometryEntityBuilder 并读写 GDTGeomEntity。

---

## 三、两类关系的具体说明

### 3.1 实体视图：FCGeometryEntityBuilder ↔ 原有几何（已用）

| 原有类 | FCGeometryEntity 侧 | 关系 |
|--------|--------------------|------|
| FCGeoCommandList | FCGeometryEntityBuilder::build(FCGeoCommandList*) / rebuild(geoList, entityModel) | **输入**。Builder 从命令列表取 **getGlobalGeoCompManager()**，不执行命令。 |
| FCGlobalGeoComponentManager | FCGeometryEntityBuilder::buildFromGlobalGeoComponentManager(compMgr) 及 build() 内部 | **输入**。按类型遍历 FCGlobalGeoComponent（Solid→Domain, Surface→Boundary, Edge→Edge, Point→Point），填 ID 与 member 映射。 |
| FCGlobalGeoComponent / FCGeoComponent | fillFromComponent() 内用 getDataByIndex()、getMember() | **只读**。用现有 member 列表生成实体 ID，并写入 FCGeometryEntityModel 的 setSourceMemberId(level, entityId, memberId)。 |

**FCGeometryEntityModel** 与 **FCGlobalData**：

- 由 **FCGlobalDataFactory::createGeometryEntityModel()** 创建，并插入 **FCGlobalData[GDTGeomEntity]**。
- 几何重建后由 **FCGlobalDataFactory::refreshGeometryEntityModel()** 或 **FCGeometryEntityBuilder::rebuild(geoList, entityModel)** 更新，使实体层与 GDTGeom（FCGeoCommandList）一致。

结论：**FCGeometryEntityBuilder + FCGeometryEntityModel** 是“在现有几何之上的只读视图层”，数据来源是 FCGeoCommandList → FCGlobalGeoComponentManager，不改变原有命令与分组结构。

---

### 3.2 函数式 DAG：FCGeometryModule ↔ 原有几何（可选扩展）

| FCGeometryEntity | 原有几何 | 关系 |
|-----------------|----------|------|
| FCGeometryModule | 无直接依赖 | 只保存 DAG（FCGeometryTree）与构建引擎；不引用 FCGeoCommandList。 |
| FCGeometryNodeExecutor（接口在 FCGeometryEntity） | **FCGeometryCommand（OCC）** 实现该接口 | 由应用或工厂**注入**到 FCGeometryModule::setExecutor()；execute() 内将 nodeResults 的 QVariant 当作 TopoDS_Shape 等执行 Block/Union/Fillet 等。 |
| build() 的 QVariant 结果 | FCGlobalGeoComponentManager / FCGeoCommandList | **不自动写回**。若要把 DAG 求值结果纳入现有几何，需在**应用层**把结果同步到命令列表或全局分组（例如新建/更新 FCAbsGeoCommand 或直接写 FCGlobalGeoComponent）。 |

结论：**FCGeometryModule** 是另一套“定义 + 求值”的 API（DAG + build）；与现有几何的衔接点只有两点：  
① 执行器由 **FCGeometryCommand** 实现；  
② 若需要，由**应用层**把 build() 结果写回 FCGeoCommandList / FCGlobalGeoComponentManager。  
当前工程中主要使用的是 **FCGeometryEntityBuilder + GDTGeomEntity** 这条线，FCGeometryModule 的集成程度取决于是否在应用层接好了执行器与写回逻辑。

---

## 四、依赖与数据流简图

```
                    FCGlobalData
                         │
         ┌───────────────┼───────────────┐
         ▼               ▼               ▼
    [GDTGeom]      [GDTGeomEntity]    [其他槽位]
         │               ▲
         │               │ 创建/刷新
         ▼               │
  FCGeoCommandList   FCGlobalDataFactory
         │               │
         │  getGlobalGeoCompManager()
         ▼               │
  FCGlobalGeoComponentManager
         │               │
         │               │ build(geoList) / rebuild(geoList, entityModel)
         │               ▼
         │         FCGeometryEntityBuilder ──────► FCGeometryEntityModel
         │
         │  (命令执行、填充分组)
         ▼
  FCGeometryCommand (OCC)
         │
         │  可实现 FCGeometryNodeExecutor，注入到
         ▼
  FCGeometryModule (DAG + build) ──► QVariant 形状
         │
         └── 应用层可选：写回 FCGeoCommandList / FCGlobalGeoComponentManager
```

- **实线常用路径**：GDTGeom → FCGeoCommandList → FCGlobalGeoComponentManager → FCGeometryEntityBuilder → FCGeometryEntityModel → GDTGeomEntity。
- **虚线可选路径**：FCGeometryModule 使用 FCGeometryCommand 实现的 Executor 求值，结果由应用写回原有几何。

---

## 五、总结表

| 关系 | 说明 |
|------|------|
| FCGeometryEntity **不替代** 原有几何 | FCGeoCommandList、FCGlobalGeoComponent 仍是几何的权威数据与分组。 |
| FCGeometryEntityBuilder **只读** 原有几何 | 从 FCGeoCommandList / FCGlobalGeoComponentManager 构建 FCGeometryEntityModel，不修改命令列表与分组。 |
| FCGeometryEntityModel **存于** FCGlobalData | 槽位 GDTGeomEntity；创建与刷新由 FCGlobalDataFactory 与几何重建处（如 FCCubeInfoWidget）调用 Builder。 |
| FCGeometryModule（DAG）**与原有几何解耦** | 不直接依赖 FCGeometryInterface；通过 FCGeometryNodeExecutor 由 FCGeometryCommand 实现，build() 结果需应用层写回才进入 GDTGeom/FCGlobalGeoComponent。 |
| 实体 ID 与 member 的对应 | FCGeometryEntityModel 的 getSourceMemberId(level, entityId) 对应 FCGlobalGeoComponent 的 member 索引，便于与 FCGeoToMeshMapper 等衔接。 |

因此：**FCGeometryEntity 是“在原有几何管理之上”的实体视图层与可选 DAG 建模层，与原有几何管理类是“消费/扩展”关系，不是替代关系。**
