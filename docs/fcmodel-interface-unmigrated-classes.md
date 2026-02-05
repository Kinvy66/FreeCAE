# FCModelInterface 迁移类清单

本文档记录从 **ignoreFiles\AppFlow\FITK_Interface\FITKInterfaceModel** 移植到 **src\FCModelInterface** 的 FITK 类对照与说明。**当前阶段所列类均已迁移完成**，未迁移类章节保留作历史参考与移植说明。

---

## 一、已迁移类对照（参考）

| 原 FITK 类 | 现 FC 类 | 说明 |
|------------|----------|------|
| FITKModelEnum | FCModelEnum | 模型/单元/集合/维度等枚举 |
| FITKAbstractModel | FCAbstractModel | 模型抽象基类 |
| FITKAbstractModelComponent | FCAbstractModelComponent | 模型组件抽象 |
| FITKNodeList | FCNodeList | 节点列表 |
| FITKAbstractMesh | FCAbstractMesh | 网格抽象基类 |
| FITKAssembly | FCAssembly | 装配体 |
| FITKAbstractAssInstance | FCAbstractAssInstance | 装配实例 |
| FITKModelSet | FCModelSet | 集合 |
| FITKComponentManager | FCComponentManager | 组件/集合管理器 |
| FITKMeshSurface | FCMeshSurface | 网格表面抽象 |
| FITKModelSurface | FCModelSurface | 模型表面抽象 |
| FITKAbstractGeoModel | FCAbstractGeoModel | 几何模型抽象（含 FCGeoComponent/FCGeoComponentManager） |
| FITKAbstractPart | FCAbstractPart | 部件抽象 |
| FITKAbsAlgorithmTools / FITKAbsInterfaceFactory | FCAbsAlgorithmTools / FCInterfaceAlgorithmFactory | 算法工具与工厂 |
| FITKMeshSurfaceElement | FCMeshSurfaceElement | 网格单元表面 |
| FITKMeshSurfaceNode | FCMeshSurfaceNode | 网格节点表面 |
| FITKGeometrySurface | FCGeometrySurface | 几何模型表面 |
| FITKCoordinateSystem / FITKCoordinateSystemManager | FCCoordinateSystem / FCCoordinateSystemManager | 坐标系及管理器 |
| FITKComponentCreator | FCComponentCreator | 部件/集合创建器 |
| FITKAbstractAssemblyElement / FITKAssemblyElementVertex / FITKAssemblyElementLine / FITKAssemblyElementSet | FCAbstractAssemblyElement / FCAssemblyElementVertex / FCAssemblyElementLine / FCAssemblyElementSet | 装配单元及集合 |

---

## 二、原“未迁移”类列表及说明（均已迁移，以下作参考）

### 1. 单元（Element）相关

| 原 FITK 类 | 性质 | 说明 |
|------------|------|------|
| **FITKAbstractElement** | 抽象接口 | 单元抽象基类：单元类型、节点数、节点 ID 列表、单元 ID 等。派生类实现具体单元形状。 |
| **FITKElementList** | 具体实现 | 单元列表容器，被网格持有，管理单元增删查及 ID/索引映射。 |
| **FITKElementFactory** | 工厂 | 根据 FCModelEnum::FITKEleType 创建对应单元类型实例。 |
| **FITKElementLine** | 具体单元 | 线单元（Line2/Line3）。 |
| **FITKElementTri** | 具体单元 | 三角形单元（Tri3/Tri6）。 |
| **FITKElementQuad** | 具体单元 | 四边形单元（Quad4/Quad8）。 |
| **FITKElementTet** | 具体单元 | 四面体单元（Tet4/Tet10）。 |
| **FITKElementWedge** | 具体单元 | 楔形单元（Wedge6）。 |
| **FITKElementHex** | 具体单元 | 六面体单元（Hex8/Hex20）。 |
| **FITKElementPolygon** | 具体单元 | 多边形单元。 |

**移植建议**：先实现 `FCAbstractElement`、`FCElementList`、`FCElementFactory`，再按需实现各 `FCElementLine`、`FCElementTri` 等；均依赖 `FCNodeList` 与 `FCModelEnum::FITKEleType`，不依赖 FITKCore。

---

### 2. 网格具体实现

| 原 FITK 类 | 性质 | 说明 |
|------------|------|------|
| **FITKUnstructuredMesh** | 具体实现 | 非结构网格，实现 FITKAbstractMesh；内部持有一个 FITKNodeList 和 FITKElementList，提供节点/单元访问、维度、边长/法向等几何计算。依赖 Core::FITKNode、FITKVec3DAlg，移植时改用 FC::FCNode、FCVec3Alg。 |
| **FITKStructuredMesh** | 具体实现 | 结构网格，实现 FITKAbstractMesh；基于规则拓扑（如 I×J×K）。 |
| **FITKParticalMesh** | 具体实现 | 粒子网格，实现 FITKAbstractMesh。 |
| **FITKUnstructuredMeshTopo** | 具体实现 | 非结构网格拓扑：节点-单元邻接、边表等；用于网格查询与算法。依赖 Core::FITKNode（移植为 FC::FCNode）。 |

**移植建议**：`FCUnstructuredMesh` 依赖 `FCNodeList`、`FCElementList`、`FCVec3Alg`；先完成单元与节点列表再实现网格与 Topo。

---

### 3. 表面/几何具体表示

| 原 FITK 类 | 性质 | 说明 |
|------------|------|------|
| **FITKMeshSurfaceElement** | 具体实现 | 网格表面上的单元表示；与 FITKMeshSurface 配合，描述表面由哪些单元/面组成。可能依赖 FITKDataRepo。 |
| **FITKMeshSurfaceNode** | 具体实现 | 网格表面上的节点表示；与 FITKMeshSurface 配合。可能依赖 FITKDataRepo。 |
| **FITKGeometrySurface** | 具体实现/接口 | 几何面（如 OCC 面）的封装；继承自 FITKAbstractModelComponent，用于几何模型中的面。可能依赖 OCC 或 FITK 几何内核。 |
| **FITKModelSurface** | 已迁为 FCModelSurface | 模型表面抽象已迁移；FITK 中尚有与 DataRepo、集合 ID 等相关的具体逻辑，可按需在 FCModelSurface 子类中补全。 |

**移植建议**：若需完整“网格表面”数据，可移植 `FCMeshSurfaceElement`、`FCMeshSurfaceNode`；几何面若依赖 OCC 等，需在 FCGeometry 或单独模块中实现。

---

### 4. 坐标系与组件创建

| 原 FITK 类 | 性质 | 说明 |
|------------|------|------|
| **FITKCoordinateSystem** | 具体数据对象 | 坐标系：原点、X 方向、Y 方向等；继承 FITKAbstractNDataObject，用于装配/几何中的局部坐标系。依赖 Core::FITKPoint、FITKAbstractDataManager。 |
| **FITKComponentCreator** | 工厂/工具 | 创建组件、集合等，可能封装“按类型创建集合/表面”的逻辑。依赖 Core::FITKAbstractObject、FITKDataRepo。 |

**移植建议**：`FCCoordinateSystem` 可用 `FCAbstractNamedDataObject` + `FCPoint` 表示原点与方向；`FCComponentCreator` 可做成无状态工具类，仅依赖 FCDataRepo。

---

### 5. 装配相关

| 原 FITK 类 | 性质 | 说明 |
|------------|------|------|
| **FITKAssElement** | 具体实现 | 装配单元集合（Assembly Element Set）；表示装配体中某类单元集合，用于载荷/边界等。依赖 FITKAbstractDataObject、FITKDataRepo。 |

**移植建议**：可迁为 `FCAssElement`，继承 `FCAbstractModelComponent` 或相应集合基类，仅使用 FCData 与 FCDataRepo。

---

### 6. 注释掉/未启用

| 原 FITK 类 | 说明 |
|------------|------|
| **FITKTriPatchesProcess** | .pri 中已注释，未启用；若为三角片/网格处理工具，可后续按需实现。 |

---

## 三、依赖关系简图（均已迁移）

```
FCAbstractMesh（已迁移）
    ├── FCUnstructuredMesh   [已迁移]
    ├── FCStructuredMesh     [已迁移]
    └── FCParticalMesh       [已迁移]

FCAbstractElement [已迁移]
    ├── FCElementLine, FCElementTri, FCElementQuad, …
    └── FCElementList, FCElementFactory [已迁移]

FCMeshSurface（已迁移）
    ├── FCMeshSurfaceElement [已迁移]
    └── FCMeshSurfaceNode    [已迁移]

FCModelSurface（已迁移）
    └── FCGeometrySurface    [已迁移]

FCCoordinateSystem / FCCoordinateSystemManager [已迁移]
FCComponentCreator [已迁移]
FCAssemblyElementSet / FCAbstractAssemblyElement 等 [已迁移]
```

---

## 四、移植时注意事项

1. **不再依赖 FITKCore**：所有 `Core::FITK*` 替换为 `FC::FC*`（如 FCNode、FCPoint、FCVec3、FCAbstractNamedDataObject、FCAbstractDataManager、FCDataRepo、FCVec3Alg）。
2. **命名与命名空间**：`FITK*` → `FC*`，`Interface` → `FC`，保持与现有 FCModelInterface 一致。
3. **API 宏**：`FITKInerfaceModelAPI` → `FCMODELINTERFACE_API`，`FITKCLASS` → `FC_CLASS(FC, FCClassName)`（见 FCData/FCMacros.h）。
4. **仓库与类型查询**：`FITKDATAREPO->getTDataByID<T>(id)` → `FCDataRepo::instance()->getDataAs<T>(id)`。
5. **几何/网格算法**：向量与点运算统一使用 `FCData/FCVec3Alg.h`（Distance、Subtract、CrossProduct、RotateAroundAxis 等）。

---

## 五、建议移植顺序

1. **单元层**：FCAbstractElement → FCElementList → FCElementFactory → FCElementLine / FCElementTri / …（按需）。
2. **网格层**：FCUnstructuredMesh（依赖 FCNodeList、FCElementList、FCVec3Alg）→ FCUnstructuredMeshTopo。
3. **表面数据**：FCMeshSurfaceElement、FCMeshSurfaceNode（若需要完整表面数据）。
4. **辅助**：FCCoordinateSystem、FCComponentCreator、FCAssElement。
5. **几何面**：FCGeometrySurface 视是否引入 OCC 等几何内核再决定放在 FCModelInterface 还是 FCGeometry 模块。

---

*文档生成自 FITKInterfaceModel.pri 与 src/FCModelInterface 现状，如有新增 FITK 类或迁移进度变更，请同步更新本文档。*
