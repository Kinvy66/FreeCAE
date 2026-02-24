# 现有模块是否适合实现 COMSOL 式函数式模型管理

本文档分析：以 **FCGeoCommandList**、**FCGeometryEntity**、**FCGeometryCommand** 为代表的现有几何模块，是否适合实现 COMSOL 那种**函数式/声明式**的几何模型管理。

---

## 一、COMSOL 式“函数式模型管理”指什么

- **定义层**：用户通过**操作序列/DAG**描述几何（如“块 A”“块 B”“A 与 B 并”“对结果倒角”），只存**操作类型 + 参数 + 依赖**，不把“某次 Build 的 Face ID”当作持久化依据。
- **求值**：按依赖顺序（拓扑序）执行节点，得到最终形状；支持**改参数 → 标记下游 dirty → 仅重算受影响部分**。
- **选择**：Fillet 等操作用**规则**（如“选某类边”）在每次 Build 后求值，而不是写死拓扑 ID，避免几何重建后 ID 变化导致选择失效。
- **实体视图**：Build 后得到**稳定的 Domain/Boundary/Edge/Point** 列表与 ID，供网格、物理、后处理统一选择（类似 Form Union 后的“最终化”实体层）。

对应到实现上需要：**DAG 定义 + 拓扑序执行 + 每节点结果缓存 + dirty 传播与局部重建 + Selection 规则 + 实体视图（Domain/Boundary/Edge/Point）**。

---

## 二、现有模块与上述需求的对应关系

| COMSOL 式需求 | 现有模块 | 适配情况 |
|---------------|----------|----------|
| **定义：操作 + 参数 + 依赖（DAG）** | FCGeometryModule + FCGeometryTree + FCGeoNode | ✅ **已具备**。addBlock/addUnion/addFillet、FCGeoNode 的 inputs、FCGeometryTree 的 topoSort/getDownstream。 |
| **求值：拓扑序执行 + 每节点结果** | FCGeometryBuildEngine | ✅ **已具备**。build() = topoSort → applyNode → nodeResults；nodeResult(id)。 |
| **改参数 → dirty → 仅重算下游** | FCGeometryBuildEngine | ✅ **已具备**。markDirty、invalidateDownstream、buildDirty()。 |
| **选择用规则而非固定 ID** | FCSelectionRule + setSelectionRuleForNode | ✅ **已具备**。Fillet 等可绑定 rule，execute 时传入 eval(shape)。 |
| **执行具体几何（Block/Union/Fillet）** | FCGeometryNodeExecutor（由 FCGeometryCommand 实现） | ⚠️ **接口已有，实现待补**。需在 OCC 层实现 execute/supports，并把 build() 结果与现有几何衔接（若需要）。 |
| **Build 后实体视图（Domain/Boundary/Edge/Point）** | FCGeometryEntityBuilder + FCGeometryEntityModel | ✅ **已具备**。当前从 FCGlobalGeoComponentManager 构建；若 DAG 为主数据源，需在 build 完成后用**最终形状**更新或生成该视图。 |
| **定义层：命令列表（历史）** | FCGeoCommandList + FCGeometryCommand | ❌ **非函数式**。顺序/树形命令、撤销/重做、存形状/member；无显式 DAG、无“仅存操作不存实体”的语义。 |

结论：**“函数式”能力已经集中在 FCGeometryEntity 的 DAG 分支（FCGeometryModule + Tree + BuildEngine + SelectionRule）**，且**实体视图**（FCGeometryEntityBuilder + FCGeometryEntityModel）也具备；**不适合**做函数式定义的是 **FCGeoCommandList 这一套**，它是历史式/命令式。

---

## 三、是否“适合”实现 COMSOL 式函数式管理

**总体结论：适合，但当前是“双轨并存”；要完整走通 COMSOL 式，需要把 DAG 作为主数据源并补全衔接。**

### 3.1 已经适合的部分

- **FCGeometryModule + FCGeometryTree + FCGeometryBuildEngine + FCGeoNode + FCSelectionRule**  
  直接对应 COMSOL 式的“定义（DAG）+ 求值 + 缓存 + dirty + 规则选择”，**抽象层面完全适合**。
- **FCGeometryEntityModel + FCGeometryEntityBuilder**  
  对应“Build 后的实体视图”（Domain/Boundary/Edge/Point），**适合**做网格/物理选择的统一接口。

### 3.2 要真正跑通 COMSOL 式还需补的环节

1. **FCGeometryNodeExecutor 的 OCC 实现**  
  在 FCGeometryCommand 中实现 execute(node, nodeResults, selectionRule)、supports(type)，使 DAG 能在本工程内实际执行；否则函数式管线“缺执行器”，只能当设计存在。

2. **DAG 作为几何的“主数据源”**  
   当前主数据源是 FCGeoCommandList（命令列表）。若要做成 COMSOL 式，需要：  
   - 要么以 **FCGeometryModule 的 DAG（FCGeometryTree/FCGeoNode）** 为主定义，并做 **DAG 的序列化/反序列化**（存盘、加载）；  
   - 要么在应用层把“命令列表”解释为 DAG（命令间依赖需显式或可推导），再驱动 FCGeometryBuildEngine——语义和复杂度都会增加。  
   更干净的做法是：**新建/编辑几何时用 FCGeometryModule 的 add*/updateNode**，持久化存 DAG，启动时从 DAG 加载并 build()。

3. **Build 结果与实体视图、全局数据的衔接**  
   - build() 得到的是 QVariant（如 TopoDS_Shape），**不会自动**写入 FCGlobalGeoComponentManager 或 FCGeometryEntityModel。  
   - 要像 COMSOL 那样“Build 后得到实体列表”，需要：在 build/buildDirty 完成后（例如在 buildFinished 信号里），用**最终形状**去更新 **FCGlobalGeoComponentManager**（或等价结构），再调用 **FCGeometryEntityBuilder** 生成/刷新 **FCGeometryEntityModel**，并写入 **GDTGeomEntity**。  
   - 文档《几何数据管理-函数式设计对比》中“与 FCGeometryEntityModel 联动”已点出这一点。

4. **与 FCGeoCommandList 的职责划分**  
   - 若采用“DAG 为主”：FCGeoCommandList 可退居为**兼容/导入/展示**（例如把既有命令列表转成 DAG，或只读显示），撤销/重做可基于 DAG 的节点操作（如 removeNode、updateNode）而非命令栈。  
   - 若暂时保留“命令列表为主”：则函数式 DAG 仅作为**可选**或**实验**路径，COMSOL 式体验不完整。

### 3.3 FCGeoCommandList 与 COMSOL 式的错位

- **FCGeoCommandList**：顺序/树形**命令历史**，每条命令产生形状并可能写入 FCGlobalGeoComponent；撤销/重做通过移动命令在 undo/redo 列表间实现。  
- **COMSOL 式**：**声明式 DAG**（节点 + 依赖），不依赖“历史顺序”而是“依赖图”；求值 = 拓扑序执行；修改 = updateNode + buildDirty。  
因此：**用“命令列表”这一套直接实现 COMSOL 式函数式管理并不适合**；适合的是 **FCGeometryEntity 里的 DAG 分支**，命令列表可作为另一套并存的入口或数据来源。

---

## 四、总结表

| 问题 | 结论 |
|------|------|
| 现有模块**整体**是否适合做 COMSOL 式函数式管理？ | **部分适合**：FCGeometryEntity 的 DAG + 构建引擎 + 实体视图**适合**；FCGeoCommandList 是命令式，**不适合**作为函数式定义层。 |
| FCGeometryModule（DAG）是否适合？ | **适合**。已有 add*/updateNode、build/buildDirty、SelectionRule、dirty 传播，抽象与 COMSOL 式一致。 |
| FCGeoCommandList 是否适合？ | **不适合**做“函数式定义”。适合保留为传统 CAD 式命令历史；若要做 COMSOL 式，应以 DAG 为主、命令列表为辅或只读。 |
| 还缺什么才能完整跑通？ | ① OCC 实现 FCGeometryNodeExecutor；② DAG 持久化与作为主数据源；③ build() 结果写回 FCGlobalGeoComponent/FCGeometryEntityModel 并刷新 GDTGeomEntity。 |

**一句话**：**适合实现 COMSOL 式函数式模型管理的，是 FCGeometryEntity 里的 DAG 分支（FCGeometryModule + Tree + BuildEngine + SelectionRule）以及实体视图（FCGeometryEntityBuilder + FCGeometryEntityModel）；现有命令列表（FCGeoCommandList）不适合作为函数式定义层，但可与 DAG 并存。** 把 DAG 设为主路径、补全执行器与持久化、并接好 build 结果到实体视图与全局数据，即可在现有模块上完整实现 COMSOL 式的函数式几何管理。
