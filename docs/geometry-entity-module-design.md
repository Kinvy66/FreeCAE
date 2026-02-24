# FreeCAE 几何模块分析与 COMSOL 式运行期几何管理方案

**状态**：运行期几何实体层已实现，见工程 `src/FCGeometryEntity` 与全局数据槽位 `GDTGeomEntity`；几何变更后需调用刷新接口以保持实体层与命令列表一致（见下文「关联更新」）。

## 一、FreeCAE 中与几何创建/管理相关的模块

### 1.1 模块划分与职责

| 模块 | 路径 | 职责 |
|------|------|------|
| **FCData** | src/FCData | 全局数据容器 `FCGlobalData`（GDTGeom 槽位）、数据对象基类、FCDataRepo |
| **FCGeometryInterface** | src/FCGeometryInterface | 几何接口层：命令基类 `FCAbsGeoCommand`、命令列表 `FCGeoCommandList`、基准/草图、**全局几何分组** `FCGlobalGeoComponent` / `FCGlobalGeoComponentManager` |
| **FCGeometryCommand** | src/FCGeometryCommand | 几何实现层（OCC）：具体命令（Box/Extrude/Boolean 等）、形状代理、从 OCC 形状填充 `FCGlobalGeoComponent` |
| **FCModelInterface** | src/FCModelInterface | 模型抽象：`FCAbstractGeoModel`、`FCGeoComponent`（类型 + member 列表）、`FCModelSet`、`FCComponentManager`，枚举 `FITKModelSetType`（Point/Edge/Surface/Solid） |
| **FCMeshInterface** | src/FCMeshInterface | 几何-网格映射 `FCGeoToMeshMapper`（geo 面/边/体/节点 ID ↔ 网格单元） |
| **FCMeshGenInterface** | src/FCMeshGenInterface | 网格生成参数与驱动注册，几何网格尺寸 `FCGeometryMeshSizeManager` 等 |

### 1.2 当前运行期几何数据流

```
FCGlobalData::_gData[GDTGeom]
        ↓
FCGeoCommandList（由 FCGlobalDataFactory::createGeoData() 创建并插入）
        ↓
  ├─ FCGeoCommandManager：根命令列表（FCAbsGeoCommand*）
  ├─ FCDatumList：基准
  ├─ FCGlobalGeoComponentManager：按类型（Point/Edge/Surface/Solid）的 FCGlobalGeoComponent
  └─ 各 FCGlobalGeoComponent 内：FCGeoComponent，存 member（子形状索引列表）

下游使用方式：
  网格生成：FCMeshGenInterface 等通过几何文件或 API 从“当前几何”生成网格；
  几何-网格映射：FCGeoToMeshMapper 用 geo 面/边/体/节点 ID 建立与网格的对应。
```

### 1.3 与 COMSOL 的差异（当前缺口）

- **无“最终化”步骤**：没有显式的 Form Union/Form Assembly，没有从“多对象”合并为“单对象 + 稳定实体列表”的环节。
- **下游契约不统一**：网格/物理选几何时依赖的是“命令/特征”或 **FCGlobalGeoComponent** 的按类型分组（FMSPoint/FMSEdge/FMSSurface/FMSSolid），而不是统一的 **Domain/Boundary/Edge/Point** 实体层级与稳定 ID。
- **无实体级稳定索引**：几何重建后，全局分组里的 member 序号可能变化，没有保证“同一逻辑面/边/体”在两次 build 间保持同一 ID 的机制（即 COMSOL 的 associative 实体层）。

因此，**可以**在现有架构上增加一个**独立模块**，在不替换现有几何命令体系的前提下，实现类似 COMSOL 的“运行期几何实体层”管理。

---

## 二、新增模块：运行期几何实体层（FCGeometryEntity）【已实现】

- **库**：`src/FCGeometryEntity`，依赖 FCData、FCGeometryInterface、FCModelInterface。
- **全局槽位**：`FCGlobalDT::GDTGeomEntity`，由 `FCGlobalDataFactory::createGeometryEntityModel()` 在 `createGlobalData()` 时创建并插入。
- **刷新**：几何重建后调用 `FCGlobalDataFactory::refreshGeometryEntityModel(globalData)` 或 `FCGeometryEntityBuilder::rebuild(geoList, entityModel)` 更新实体层。

### 2.1 目标

- 在**现有几何命令列表（FCGeoCommandList）**之上，增加一层**只读的、实体化的几何视图**。
- 提供 **Domain / Boundary / Edge / Point**（或与现有 FCModelEnum 对应的体/面/边/点）的**稳定索引**和**选择接口**，供网格、物理、后处理使用。
- 几何命令列表仍负责“创建与编辑”；本模块负责“从当前几何 build 出实体层并在几何重建时更新”，实现类似 COMSOL 的 **Geometry Sequence → Build → Form Union/Assembly → 实体层** 的机制。

### 2.2 在工程中的位置

- **新增库**：例如 `FCGeometryEntity`（或 `FCGeometryRuntime`），依赖 `FCData`、`FCGeometryInterface`、`FCModelInterface`，可选依赖 `FCGeometryCommand`（若从 OCC 形状直接遍历生成实体列表）。
- **不替代** FCGeoCommandList、FCGlobalGeoComponent，而是**基于其输出**构建实体层。

### 2.3 核心概念设计

1. **GeometryEntityModel（几何实体模型）**
   - 表示一次“Build”后的几何实体视图。
   - 持有：
     - **Domains**：体（3D）或面（2D）或线（1D），每个带稳定 ID 与可选名称。
     - **Boundaries**：面（3D）或边（2D）或点（1D）。
     - **Edges**：边（3D）或点（2D）。
     - **Points**：点。
   - 可与 FCModelEnum::FITKModelSetType（FMSSolid/FMSSurface/FMSEdge/FMSPoint）或现有 FCGlobalGeoComponent 的 member 建立映射，以便从现有实现填充。

2. **GeometryEntityBuilder（几何实体构建器）**
   - 输入：当前 `FCGeoCommandList`（或从 FCGlobalData 取 GDTGeom 得到）。
   - 行为：遍历根命令生成的形状，或利用 **FCGlobalGeoComponentManager** 已按类型分组的 Face/Edge/Solid 等，生成 **GeometryEntityModel**（Domain/Boundary/Edge/Point 列表及稳定 ID）。
   - 可选：实现“Form Union”语义（合并多个体为一个对象，并重新编号 Domain/Boundary/Edge/Point）。

3. **与 FCGlobalData 的集成**
   - 方式 A：在 **FCGlobalData** 中增加槽位（如 GDTGeomEntity），由 **FCAbstractGlobalDataFactory** 在 createGlobalData 时创建并维护一个 **GeometryEntityModel**（或将其挂在现有几何数据对象上）。
   - 方式 B：不新增槽位，由“需要实体层的模块”（如网格、物理）在需要时通过 **FCGeometryEntityBuilder** 从 `FCGlobalData::getData<FCGeoCommandList>(GDTGeom)` 构建一次 GeometryEntityModel，并缓存到合适位置（例如全局单例或 FCGlobalData 的扩展数据）。

4. **选择接口**
   - 提供按 **Entity Level**（Domain/Boundary/Edge/Point）和 **ID 或名称** 的查询接口，例如：
     - `getDomainIds()`, `getBoundaryIds()`, `getEdgeIds()`, `getPointIds()`
     - `getBoundariesOfDomain(domainId)`, `getEdgesOfBoundary(boundaryId)` 等
   - 供网格尺寸、物理边界条件、后处理选择等统一使用。

### 2.4 与现有组件的衔接

- **FCGlobalGeoComponentManager**：已有按类型的几何分组（面/边/体等），**GeometryEntityBuilder** 可优先基于其 member 列表和类型生成 Domain/Boundary/Edge/Point，减少重复遍历。
- **FCGeoToMeshMapper**：当前按 geo 面/边/体/节点 ID 映射；若实体层与现有 ID 一致或提供映射表，可继续复用；若实体层采用新 ID 体系，则需在 FCGeoToMeshMapper 或上层维护“实体 ID ↔ 原 geo ID”的映射。
- **FCMeshGenInterface / FCGeometryMeshSizeManager**：可改为支持“按 Domain/Boundary 选中的区域”施加尺寸，而不仅按命令或组件。

### 2.5 实现步骤建议

1. **新建库 FCGeometryEntity**  
   - CMake 与现有模块一致，依赖 FCData、FCGeometryInterface、FCModelInterface。

2. **定义 GeometryEntityModel**  
   - 数据结构：Domain/Boundary/Edge/Point 的 ID 列表及层级关系（如 boundary 属于哪个 domain）；可选名称、颜色等。

3. **实现 GeometryEntityBuilder**  
   - 从 `FCGeoCommandList` 取根命令 → 取形状（若为 OCC，可从 FCAbsGeoShapeAgent 取 TopoDS_Shape）或从 **FCGlobalGeoComponentManager** 取各类型 FCGeoComponent 的 member；  
   - 按 3D/2D/1D 将 Solid→Domain、Face→Boundary、Edge→Edge、Vertex→Point 填入 GeometryEntityModel，并生成稳定 ID（例如按遍历顺序或拓扑哈希）。

4. **集成到 FCGlobalData / FCGlobalDataFactory**  
   - 在 createGeoData 或 createGlobalData 中，在创建几何数据后，创建并 build 一次 GeometryEntityModel，写入 FCGlobalData 的新槽位或扩展；  
   - 或在几何“Build”完成时（若现有有统一 rebuild 入口）调用 Builder 更新实体模型。

5. **暴露选择 API**  
   - 在 FCGeometryEntity 中提供按 Entity Level + ID 的查询接口，供 FCMeshGenInterface、物理设置、后处理等调用。

6. **关联更新（Associative）**  
   - 当用户修改几何（撤销/重做、编辑参数、增删命令）并触发几何重建时，重新执行 GeometryEntityBuilder 更新 GeometryEntityModel；若采用“稳定 ID”策略（如按拓扑或名称），可尽量保持同一逻辑实体的 ID 不变，以便物理/网格选择保持有效。
   - **已实现**：`FCGlobalDataFactory::refreshGeometryEntityModel(FCGlobalData* globalData)` 根据当前 `GDTGeom`（FCGeoCommandList）重建 `GDTGeomEntity`（FCGeometryEntityModel）。在「构建」「构建所有」等几何重建后应调用此接口（或直接使用 `FCGeometryEntityBuilder::rebuild`）；当前在单命令构建（FCCubeInfoWidget::onBuildClicked）与「构建所有」（onBuildAllClicked）后会刷新实体层。

---

## 三、结论

- FreeCAE **已有**完整的“几何创建”体系（FCGeometryInterface + FCGeometryCommand）和运行期几何数据管理（FCGlobalData + FCGeoCommandList + FCGlobalGeoComponent），与**传统建模软件**一致（以命令和实体保存）。
- **可以**在不破坏现有架构的前提下，**新增一个模块**（建议 **FCGeometryEntity**），实现类似 COMSOL 的机制：
  - 以当前几何命令列表（或全局几何分组）为输入，**构建“最终化”的几何实体视图**（Domain/Boundary/Edge/Point + 稳定 ID）；
  - 为网格、物理、后处理提供**统一的实体级选择接口**；
  - 在几何重建时**更新实体层**，实现关联性。
- 实现时优先复用 **FCGlobalGeoComponentManager** 与 **FCModelInterface** 的类型与集合概念，再在 FCGlobalData 侧挂接实体模型与构建入口即可。
