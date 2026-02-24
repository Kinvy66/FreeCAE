# 函数式几何完全替代 FCGeoCommandList 说明

**状态**：已实施。GDTGeom 槽位使用 **FCGeometryDAGData** 作为几何主数据源，全局几何管理类内部以 **几何序列（DAG）** 管理“命令类型 + 参数”，不再使用 FCGeoCommandList。

---

## 一、设计目标

- **COMSOL 式函数式几何管理**：只保存几何操作与参数，不保存中间实体；支持 DAG 依赖、dirty 传播与局部重建、Selection 规则。
- **完全替代 FCGeoCommandList**：全局几何由 **FCGeometryDAGData** 统一管理，几何序列 = DAG 节点列表，每项对应“FCAbsGeoCommand 式的命令类型 + 对应参数”。

---

## 二、全局几何管理类：FCGeometryDAGData

| 项目 | 说明 |
|------|------|
| **槽位** | `FCGlobalDT::GDTGeom`，由 `FCGlobalDataFactory::createGeoData()` 创建并插入 `FCGlobalData`。 |
| **内部几何序列** | **FCGeometryTree** 中的 DAG 节点（**FCGeoNode**）。序列中每一项 = **命令类型（FCGeoOpType）+ 参数（FCGeoParamSet）+ 依赖（inputs）**，与“FCAbsGeoCommand 命令 + 参数”一一对应。 |
| **不保存** | 不保存 FCAbsGeoCommand 对象、不保存中间 Face/Solid 实体；仅保存操作类型、参数与依赖，Build 时由 **FCGeometryBuildEngine** + **FCGeometryNodeExecutor**（OCC 等）求值。 |

### 2.1 几何序列的表示

- **FCGeoOpType**：Block、Cylinder、Sphere、Union、Difference、Intersection、Fillet、Chamfer、Import、Extrude、Revolve、Sweep 等，对应原命令类型。
- **FCGeoParamSet**：`QMap<QString, QVariant>`，如 Block 的 length/width/height，对应原命令参数。
- **inputs**：`QList<int>` 输入节点 ID，表示依赖（如 Union(a,b) 的 a、b）。

通过 **FCGeometryModule** 的 `addBlock(params)`、`addUnion(a,b)`、`updateNode(id, params)`、`build()` / `buildDirty()` 等 API 维护该序列并求值。

### 2.2 提供的 API

- **module()**：获取 FCGeometryModule（DAG + 构建引擎），用于 add* / updateNode / build。
- **getGlobalGeoCompManager()**：Build 后由 buildResultFiller 填充，供 FCGeometryEntityBuilder 构建实体视图（Domain/Boundary/Edge/Point）。
- **getGeometrySequence()**：返回当前几何序列（所有 FCGeoNode 的快照），便于 UI 或持久化展示。
- **serialize() / deserialize()**：DAG 的 JSON 序列化/反序列化，用于存盘与加载。

---

## 三、与 FCGeoCommandList 的对照

| 原 FCGeoCommandList | 现 FCGeometryDAGData |
|--------------------|----------------------|
| 根命令列表 getRootCommandList() | module()->tree()->nodeIds() + node(id)，或 getGeometrySequence() |
| 命令对象 FCAbsGeoCommand* | DAG 节点 FCGeoNode（type + params + inputs） |
| 撤销/重做列表 | 由 DAG 的 updateNode/removeNode 等表达修改；可在此基础上实现历史/撤销 |
| getGlobalGeoCompManager() | getGlobalGeoCompManager()（Build 后填充） |
| 命令执行 | FCGeometryBuildEngine + FCGeometryNodeExecutor（OCC）按拓扑序执行 |

---

## 四、使用处变更摘要

- **FCGlobalDataFactory::createGeoData()**：创建 **FCGeometryDAGData**（已实施）。
- **FCProjectTreeWidget::updateGeometryItems()**：仅从 GDTGeom 取 **FCGeometryDAGData**，用 DAG 节点填充树，已移除对 FCGeoCommandList 的 fallback。
- **FCCubeInfoWidget**：onBuildAllClicked、rebuildGeometryEntityModel 仅使用 **FCGeometryDAGData**，已移除对 FCGeoCommandList 的引用。
- **FCGeometryEntityBuilder**：仍保留 `build(FCGeoCommandList*)` / `rebuild(geoList, entityModel)` 接口，用于兼容旧数据或导入；主路径为从 **FCGeometryDAGData::getGlobalGeoCompManager()** 构建/刷新 FCGeometryEntityModel。

---

## 五、持久化

- **FCGeometryDAGData::serialize()**：将 DAG 节点列表序列化为 JSON（version、nodes：id/type/name/params/inputs）。
- **FCGeometryDAGData::deserialize()**：从 JSON 恢复 DAG，并调用 tree()->setNextNodeId(maxId) 保证后续新节点 ID 不冲突。

应用层在存盘/加载时应对 GDTGeom 中的 FCGeometryDAGData 调用 serialize/deserialize，并在加载后调用 ensureBuild() 与 refreshGeometryEntityModel() 以恢复几何与实体视图。

---

## 六、参考文档

- `geometry-entity-module-design.md`：几何实体层与 DAG 在工程中的位置。
- `COMSOL-style-functional-geometry-suitability.md`：COMSOL 式需求与现有模块对应。
- `几何数据管理-函数式设计对比.md`：函数式能力与 FCGeometryEntity 已实现内容。
- `函数式几何模块设计指导文档.md`：DAG + 重建引擎 + 规则选择的总体设计。
