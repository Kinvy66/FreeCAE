# FITKInterfaceGeometry 模块分析与迁移说明

## 一、模块功能概述

**FITKInterfaceGeometry** 是几何建模的**接口层**，主要功能包括：

1. **几何命令体系**：以“命令”(Command) 为单位的建模操作，如创建体（Box/Cylinder/Sphere）、布尔运算、导入/导出、变换、草图、基准、分割/缝合等。
2. **虚拓扑 (Virtual Topology)**：对几何形状（点/边/面/体等）做逻辑分组与引用，便于后续选择、载荷施加和网格关联。
3. **形状代理 (Shape Agent)**：每个几何命令对应一个 ShapeAgent，负责生成/更新底层几何、构建虚拓扑、三角化与可视化数据。
4. **基准元素 (Datum)**：点、线、面等参考几何，用于草图与建模约束。
5. **部件与实例**：Part（部件，含多 Body）、Instance（装配实例）、导入部件等。
6. **工厂与扩展**：通过工厂按命令类型创建命令/基准，支持多几何内核（OCC/Parasolid 等）扩展。

## 二、目录与文件分组

| 分组 | 文件示例 | 说明 |
|------|----------|------|
| **API/枚举** | FITKInterfaceGeometryAPI.h, FITKGeoEnum.h | 导出宏、几何引擎/命令/基准/虚拓扑类型枚举 |
| **命令基类** | FITKAbsGeoCommand.h/cpp, FITKGeoCommandProp.h/cpp | 抽象命令、命令属性、命令管理器 |
| **形状代理与虚拓扑** | FITKAbsGeoShapeAgent.h/cpp, FITKAbsVirtualTopo.h/cpp, FITKVirtualTopoManager.h/cpp, FITKAbsVirtualTopoMapper | 形状代理、虚拓扑对象与按类型管理 |
| **基准** | FITKAbsGeoDatum.h/cpp, FITKAbsGeoReferencePoint/Line/Plane | 基准抽象与具体基准（点/线/面） |
| **工厂** | FITKGeoInterfaceFactory.h/cpp | 单例工厂：按类型创建命令/基准，注册虚拓扑与 GeomTools 创建器 |
| **部件/实例** | FITKAbsGeoPart.h/cpp, FITKAbsGeoInstance.h/cpp, FITKAbsGeoImportedPart, FITKGlobalGeoComponent | 部件、实例、全局几何组件 |
| **基础体** | FITKAbsGeoModelBox/Cylinder/Sphere | 长方体、圆柱、球 |
| **点/线/面/体** | FITKAbsGeoModelPoint/Curve/Surface/Solid | 点、曲线、曲面、实体模型命令 |
| **布尔/操作** | FITKAbsGeoOperBool, FITKAbsGeoOperNBodyUnite, FITKAbsGeoModelOperLine/Face/Solid/Compound/ImprintSolid | 布尔运算、体操作、压印、复合 |
| **导入导出** | FITKAbsGeoModelImport/Export/ExportTopos, FITKAbsGeoImportBase, FITKImportFileThread, FITKExportFileThread | 文件导入/导出、拓扑导出、后台线程 |
| **变换** | FITKAbsGeoTransformation | 平移、旋转、缩放、镜像、阵列 |
| **草图** | FITKAbsGeoSketch2D, FITKAbsGeoSketchFillet, FITKAbsGeoSketchDeleter | 2D 草图、草图倒角、草图删除 |
| **构造** | FITKAbsConstruction | 构造线 |
| **分割/缝合** | FITKAbsGeoSplitter, FITKAbsGeoStitch, FITKAbsGeoCoverEdges, FITKAbsGeoReplaceFaces | 曲线/面/体分割、缝合、覆盖面、替换面 |
| **分区** | FITKAbsGeoModelPartitionEdge/Face/Solid | 按参数/点/基准面/草图等分区 |
| **特征** | FITKAbsGeoFeature (Stiffener 等) | 肋等特征 |
| **删除** | FITKAbsGeoDelete, FITKAbsGeoMultiDelete | 删除、多选删除 |
| **工具/网格** | FITKAbsGeomTools, FITKGeometryMeshVS, FITKGeometryMeshEntity | 几何工具接口、几何网格显示、实体 |
| **虚拓扑组合** | FITKAbsGeoVirtualTopoCombine, FITKAbsGeoModelExportTopos | 虚拓扑合并、导出拓扑 |
| **其他** | FITKAbsGeoModelCut, NearestPointLine, ExtractCenter, FromMesh, RepairSmallEdges/Faces 等 | 切削、最近点线、抽中面、从网格生成、修复小边/面 |
| **Python** | PyFITKGeoBehavior, PyFITKSketch2D | Python 行为与草图绑定 |

## 三、外部依赖

| 依赖模块 | 用途 |
|----------|------|
| **FITKCore** | FITKAbstractNDataObject, FITKVarientParams, FITKAbstractDataManager, FITKAbstractDataObject |
| **FITKInterfaceModel** | FITKAbstractGeoModel, FITKModelEnum |
| **FITKAppFramework** | FITKAPP, FITKSignalTransfer（消息/日志输出） |
| **OpenCASCADE (OCC)** | 多数 .cpp 中实际几何操作（TopoDS_Shape 等），具体命令与 ShapeAgent 实现 |

迁移到 FreeCAE 时：

- **FITKCore** → **FCData**（FCAbstractNamedDataObject, FCVariantParams, FCAbstractDataManager, FCAbstractDataObject）
- **FITKInterfaceModel** → **FCModelInterface**（FCAbstractGeoModel, FCModelEnum）
- **FITKAppFramework** → 可选：通过回调或 FCMessageHandler 输出日志，或接口抽象为“无实现”
- **OCC** → 保留在具体实现层；FCGeometryInterface 仅保留接口与枚举，具体命令可在 FCGeometryCommand 或带 OCC 的子工程中实现

## 四、迁移映射（FITK → FC）

| 原 FITK 类/枚举 | 迁移后 FC 类/枚举 | 说明 |
|-----------------|-------------------|------|
| FITKInterfaceGeometryAPI | FCGEOMETRYINTERFACE_API | 导出宏 |
| FITKGeoEnum | FCGeoEnum | 几何引擎、虚拓扑类型、基准类型、命令类型、状态、构造类型、特征类型等 |
| VirtualShape | VirtualShape | 虚拓扑引用结构体（CmdId, VirtualTopoId, VirtualTopoIndex, Type） |
| FITKAbsGeoCommand | FCAbsGeoCommand | 几何命令抽象基类 |
| FITKGeoCommandManager | FCGeoCommandManager | 命令容器，继承 FCAbstractDataManager\<FCAbsGeoCommand\> |
| FITKGeoCommandProp | FCGeoCommandProp | 命令属性（颜色、数字等） |
| FITKAbsGeoShapeAgent | FCAbsGeoShapeAgent | 形状代理抽象，继承 FCAbstractGeoModel |
| FITKAbsVirtualTopoShape | FCAbsVirtualTopoShape | 虚拓扑形状代理接口 |
| FITKAbsVirtualTopo | FCAbsVirtualTopo | 虚拓扑对象，继承 FCAbstractDataObject |
| FITKVirtualTopoManager 等 | FCVirtualTopoManager | 按类型管理虚拓扑 |
| FITKAbsGeoDatum | FCAbsGeoDatum | 基准抽象，继承 FCAbsGeoCommand |
| FITKInterfaceGeometryFactory | FCGeoInterfaceFactory | 单例工厂，创建命令/基准 |
| FITKAbsGeoPart | FCAbsGeoPart | 部件命令（命令+子命令容器） |

## 五、已迁移内容（src/FCGeometryInterface）

- **FCGeometryInterfaceAPI.h**：导出宏。
- **FCGeoEnum**：几何相关枚举（引擎、虚拓扑类型、基准类型、命令类型、状态、构造类型、特征类型、基准面/轴类型等）。
- **VirtualShape**：虚拓扑引用结构。
- **FCAbsGeoCommand** / **FCGeoCommandManager**：命令抽象与管理器（依赖 FCData + FCModelInterface）。
- **FCAbsGeoShapeAgent**：形状代理抽象（继承 FCAbstractGeoModel，依赖 FCModelInterface）。
- **FCAbsVirtualTopo** / **FCAbsVirtualTopoShape** / **FCVirtualTopoManager**：虚拓扑接口与按类型管理。
- **FCGeoCommandProp**：命令属性。
- **FCAbsGeoDatum**：基准抽象。
- **FCGeoInterfaceFactory**：单例工厂，注册/创建命令与基准。
- **FCAbsGeoPart**：部件命令（继承 FCAbsGeoCommand + FCGeoCommandManager），子命令容器、自动布尔合并、removeDataObj 等重写并调用 removeFromPart。
- **FCAbsGeoInstance**：装配实例命令（继承 FCAbsGeoCommand），引用部件命令 ID，getPartCommand() 通过 FCDataRepo 获取 FCAbsGeoPart。
- **FCGeoModelManager**：几何模型管理器（FCAbstractDataManager\<FCAbstractGeoModel\>），getBoundaryBox 合并所有模型的包围盒，getDataByIndexT\<T\> 模板方法。

上述类均不依赖 FITKCore/FITKAppFramework，仅依赖 FCData；需要几何模型时依赖 FCModelInterface。**具体几何命令实现**（Box/Cylinder/Import 等）及依赖 OCC 的 ShapeAgent/GeomTools 留待 FCGeometryCommand 或后续带 OCC 的模块实现。

## 六、迁移对照总表（FITK → FC）

### 6.1 已完整或部分迁移的接口

| FITK 源文件 | FC 对应 | 说明 |
|-------------|---------|------|
| FITKAbsGeoCommand | FCAbsGeoCommand | 命令基类、管理器 |
| FITKGeoCommandProp | FCGeoCommandProp | 命令属性 |
| FITKAbsGeoShapeAgent | FCAbsGeoShapeAgent | 形状代理 |
| FITKAbsVirtualTopo / FITKVirtualTopoManager | FCAbsVirtualTopo / FCVirtualTopoManager | 虚拓扑 |
| FITKAbsGeoDatum + DatumPoint/Line/Plane | FCAbsGeoDatum + FCAbsGeoDatumPoint/Line/Plane | 基准基类 |
| FITKGeoEnum | FCGeoEnum | 枚举 |
| FITKGeoInterfaceFactory | FCGeoInterfaceFactory | 工厂 |
| FITKAbsGeoPart / FITKAbsGeoInstance | FCAbsGeoPart / FCAbsGeoInstance | 部件、实例 |
| FITKGeoModelManager | FCGeoModelManager | 模型管理器 |
| FITKAbsGeoModelBox/Cylinder/Sphere | FCGeoModelBox/Cylinder/Sphere | 基础体 |
| FITKAbsGeoDelete / FITKAbsGeoMultiDelete | FCGeoDelete / FCGeoMultiDelete | 删除 |
| FITKAbsGeoOperBool / FITKAbsGeoOperNBodyUnite | FCGeoOperBool / FCGeoOperNBodyUnite | 布尔、多体并 |
| FITKAbsGeoImportBase / FITKAbsGeoModelImport/Export | FCGeoImportBase / FCGeoModelImport/Export | 导入导出 |
| FITKAbsGeoTransformation 及子类 | FCGeoTransformation 及子类 | 平移/旋转/缩放/镜像/阵列 |
| FITKAbsGeoModelPoint 及部分子类 | FCGeoModelPoint（Point/CurveEnd/CurveEndDistance/CurveRadio/InteractionPoint） | 点及曲线相关点 |
| FITKAbsGeoModelSolid 及子类 | FCGeoModelSolid（ClosedSurface/Extrude/Revol/Sweep/MultiSection） | 实体 |
| FITKAbsGeoModelCut 及子类 | FCGeoModelCut（Extrude/Revol/Sweep/MultiSection） | 切割 |
| FITKAbsGeoModelOperSolid 部分 | FCGeoModelOperSolid（Chamfer/Fillet/Defeature/RemoveChamfer/RemoveFillet/FillHole） | 倒角/倒圆/特征移除（无 CircularHole） |
| FITKAbsGeoReferencePoint/Line/Plane 及子类 | FCGeoReferencePoint/Line/Plane 及子类 | 参考点/线/面 |
| FITKAbsGeoStitch | FCGeoStitch | 缝合 |
| FITKAbsGeoSplitter 及子类 | FCGeoSplitter（Curve/Surface/Solid） | 分割 |
| FITKAbsGeoModelPartitionEdge/Face/Solid 及部分子类 | FCGeoModelPartitionEdge/Face/Solid 及部分子类 | 边/面/体分块（部分子类） |
| FITKGeometryMeshVS | FCGeometryMeshVS | 几何网格显示桩 |

### 6.2 尚未迁移的 FITK 接口（按类别）

**草图与构造**

- FITKAbsGeoSketch2D、FITKAbsGeoSketchDeleter、FITKAbsGeoSketchFillet  
- FITKAbsConstruction、FITKAbsExtendTool  

**曲线 / 曲面 / 体（完整层次）**

- FITKAbsGeoModelCurve（整棵曲线命令树：Line/Segment/Wire/Circle/BSpline/Offset/Trimmed 等）  
- FITKAbsGeoModelSurface（整棵曲面命令树：ClosedWire/Offset/Extrude/Revol/Sweep 等）  
- FITKAbsGeoModelOperLine、FITKAbsGeoModelOperFace  
- FITKAbsGeoModelOperCompound、FITKAbsGeoModelOperImprintSolid  

**导入导出与部件**

- FITKAbsGeoModelExportTopos  
- FITKAbsGeoModelImportFlattenAssembly  
- FITKAbsGeoImportedPart  
- FITKImportFileThread、FITKExportFileThread（后台线程，偏应用层）  

**修复与替换**

- FITKAbsGeoCoverEdges、FITKAbsGeoReplaceFaces  
- FITKAbsGeoRepairSmallEdges、FITKAbsGeoRepairSmallFaces  

**其他几何命令**

- FITKAbsGeoModelExtractCenter、FITKAbsGeoModelFromMesh  
- FITKAbsGeoModelNearestPointLine  
- FITKAbsGeoSampleCurve  
- FITKAbsGeoFeature（如 Stiffener）  
- FITKAbsGeoVirtualTopoCombine  
- FITKAbsGeoModelOperSolid 中的 CircularHole  

**辅助与 Python**

- FITKAbsGeomTools、FITKAbsVirtualTopoMapper  
- FITKGeoCommandList、FITKGlobalGeoComponent  
- FITKGeometryMeshEntity  
- PyFITKGeoBehavior、PyFITKSketch2D（Python 绑定）  

**Partition 未覆盖子类**

- PartitionFace：WithCurvedPath、WithIntersectFace、WithProjectEdges  
- PartitionSolid：WithSweepEdge、WithNSidedPatch  

**Point 未覆盖子类**

- PointProjectionOnCurve、TwoCurveIntersectPoint、PointProjectionOnSurface、CurveIntersectSurface  

---

结论：**核心框架、基础体、删除、布尔、导入导出、变换、点/体/切割/体操作、参考点线面、缝合、分割、分区（边/面/体）的主要接口已移植**；**曲线/曲面完整命令树、草图、构造、修复/替换、虚拓扑组合、导出拓扑、从网格/抽中面/最近点线、CircularHole、Partition/Point 部分子类及 Python/线程/列表等辅助类尚未移植**。若需“全部移植”，可在此基础上按上述列表逐项补充接口（仍为接口层，不含 OCC 实现）。

## 七、移植注意事项

1. **日志输出**：原 `printLog` 使用 FITKAPP->getSignalTransfer()；迁移后可改为空实现或注入 `FCGeoMessageHandler` 等接口。
2. **单例**：`DeclSingleton(FITKInterfaceGeometryFactory)` 改为手写单例或 FC 侧单例宏。
3. **FITKCLASS**：改为 `FC_CLASS(FC, ClassName)`（见 FCData/FCMacros.h）。
4. **命名空间**：Interface → FC。

---

*文档与迁移对应 ignoreFiles/AppFlow/FITK_Interface/FITKInterfaceGeometry 及 src/FCGeometryInterface。最后更新：补充第六节迁移对照总表与未迁移项清单。*
