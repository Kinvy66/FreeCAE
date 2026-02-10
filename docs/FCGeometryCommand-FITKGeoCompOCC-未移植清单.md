# FCGeometryCommand 与 FITKGeoCompOCC 未移植对比清单

`src/FCGeometryCommand` 的代码移植自 `ignoreFiles/AppFlow/FITK_Component/FITKGeoCompOCC`。本文档对比两处，列出**尚未移植**的模块、文件和类。

---

## 一、整文件/整模块未移植

以下在 FITKGeoCompOCC 中存在整块实现，在 FCGeometryCommand 中**没有对应文件**。

| 源模块文件 (FITKGeoCompOCC) | FreeCAE 情况 | 说明 |
|-----------------------------|-------------|------|
| **FITKOCCModelCurve.h/cpp** | 已有 FCOCCModelCurve | 部分移植：Line/Segment/Wire/Circle/ThreePointsCircle/CircleArc/Ellipse；其余见下文「曲线类」 |
| **FITKOCCModelSurface.h/cpp** | 已有 FCOCCModelSurface | 除 CoverEdges 外已移植 ClosedWireSurface；其余见下文「曲面类」 |
| **FITKOCCReferencePoint.h/cpp** | 无 OCC 实现 | 基准点：FC 使用 FCGeometryInterface 的 FCGeoReferencePoint，未做 OCC 版 |
| **FITKOCCReferenceLine.h/cpp** | 无 OCC 实现 | 基准线（含 ByIntersectPlanes、ByRotateLine）：FC 用接口层实现 |
| **FITKOCCReferencePlane.h/cpp** | 无 OCC 实现 | 基准面（含 Offset、ThreePoints、Equation 等）：FC 用接口层实现 |
| **FITKOCCModelSketch2D.h/cpp** | 已有 FCOCCModelSketch2D | ✅ 已移植（汇总子命令边为 Compound，带形状缓存） |
| **FITKOCCModelRefine.h/cpp** | 无 FCOCCModelRefine | 模型细化未移植 |
| **FITKOCCVirtualTopoCreator.h/cpp** | 已有 FCOCCVirtualTopoCreator.h/cpp | ✅ 已移植（FCOCCTopoShape 置于同头文件，createOCCTopos 填充 FCVirtualTopoManager） |
| **FITKOCCShapeTriangulate.h/cpp** | 已有 FCOCCShapeTriangulate.h/cpp | ✅ 已移植（BRepMesh_IncrementalMesh，按包围盒 Deflection） |
| **FITKOCCGeomTools.h/cpp** | 无 FCOCCGeomTools | 几何工具未移植 |
| **FITKOCCAlgTools.h/cpp** | 无 FCOCCAlgTools | 算法工具未移植 |
| **FITKOCCComandCommon.h/cpp** | 无 FCOCCComandCommon | 命令公共逻辑未移植 |
| **FITKOCCFeature.h/cpp** | 无 FCOCCFeature | Feature 扩展未移植 |
| **FITKOCCExtendTool.h** | 无 FCOCCExtendTool | 扩展工具头文件未移植 |
| **FITKOCCDXFReader.h/cpp** | 无 FCOCCDXFReader | DXF 读取未移植 |
| **FITKOCCSampleCurve.h/cpp** | 已有 FCOCCSampleCurve | ✅ 已移植（等参/等弧长/等弦长，输出顶点 Compound） |
| **FITKOCCModelImportedPart.h/cpp** | 已有 FCOCCModelImportedPart | 已移植 |
| **FITKShapeFix_FixSmallFace.h/cpp** | 无对应 | 小面修复辅助未移植 |
| **FITKGeoCompOCCInterface.h/cpp** | 对应 FCOCCGeoCompRegister | 注册逻辑已移植，基准在 FC 层用 FCGeoReference* |

---

## 二、按类的未移植清单

### 2.1 点 (FITKOCCModelPoint → FCOCCModelPoint)

| 源类 (FITK) | FreeCAE 对应 | 状态 |
|-------------|--------------|------|
| FITKOCCModelPoint | FCOCCModelPoint | ✅ 已移植 |
| FITKOCCModelInteractionPoint | FCOCCModelInteractionPoint | ✅ 已移植 |
| FITKOCCModelCurveEnd | FCOCCModelCurveEnd | ✅ 已移植 |
| FITKOCCModelCurveEndDistance | FCOCCModelCurveEndDistance | ✅ 已移植 |
| FITKOCCModelCurveRadio | FCOCCModelCurveRadio | ✅ 已移植 |
| FITKOCCModelPointProjectionOnCurve | FCOCCModelPointProjectionOnCurve | ✅ 已移植 |
| FITKOCCModelTwoCurveIntersection | FCOCCModelTwoCurveIntersectPoint | ✅ 已移植 |
| FITKOCCModelPointProjectionOnSurface | FCOCCModelPointProjectionOnSurface | ✅ 已移植 |
| FITKOCCModelCurveIntersectionSurface | FCOCCModelCurveIntersectSurface | ✅ 已移植 |

### 2.2 曲线 (FITKOCCModelCurve → FCOCCModelCurve)

| 源类 (FITK) | FreeCAE 对应 | 状态 |
|-------------|--------------|------|
| FITKOCCModelLine | FCOCCModelLine | ✅ 已移植（update 为桩） |
| FITKOCCModelSegment | FCOCCModelSegment | ✅ 已移植 |
| FITKOCCModelWire | FCOCCModelWire | ✅ 已移植 |
| FITKOCCModelCircle | FCOCCModelCircle | ✅ 已移植 |
| FITKOCCModelThreePointsCircle | FCOCCModelThreePointsCircle | ✅ 已移植 |
| FITKOCCModelCircleArc | FCOCCModelCircleArc | ✅ 已移植 |
| FITKOCCModelEllipse | FCOCCModelEllipse | ✅ 已移植 |
| FITKOCCModelThreePointsEllipse | FCOCCModelThreePointsEllipse | ✅ 已移植 |
| FITKOCCModelEllipseArc | FCOCCModelEllipseArc | ✅ 已移植 |
| FITKOCCModelHyperbola | FCOCCModelHyperbola | ✅ 已移植 |
| FITKOCCModelThreePointsHyperbola | FCOCCModelThreePointsHyperbola | ✅ 已移植 |
| FITKOCCModelHyperbolaArc | FCOCCModelHyperbolaArc | ✅ 已移植 |
| FITKOCCModelParabola | FCOCCModelParabola | ✅ 已移植 |
| FITKOCCModelThreePointsParabola | FCOCCModelThreePointsParabola | ✅ 已移植 |
| FITKOCCModelParabolaArc | FCOCCModelParabolaArc | ✅ 已移植 |
| FITKOCCModelBeizeByControlPoints | FCOCCModelBezierByControlPoints | ✅ 已移植 |
| FITKOCCModelBsplineByThroughPoints | FCOCCModelBsplineByThroughPoints | ✅ 已移植 |
| FITKOCCModelTrimmedCurve | FCOCCModelTrimmedCurve | ✅ 已移植（桩） |
| FITKOCCModelOffsetCurve | FCOCCModelOffsetCurve | ✅ 已移植 |
| FITKOCCModelCurveProjectionOnSurface | FCOCCModelCurveProjectionOnSurface | ✅ 已移植 |
| FITKOCCModelSurfaceIntersectionSurface | FCOCCModelSurfaceIntersectionSurface | ✅ 已移植 |
| FITKOCCModelBridgeCurve | FCOCCModelBridgeCurve | ✅ 已移植 |
| FITKOCCModelSurfaceEdge | FCOCCModelSurfaceEdge | ✅ 已移植 |

### 2.3 曲面 (FITKOCCModelSurface → FCOCCModelSurface)

| 源类 (FITK) | FreeCAE 对应 | 状态 |
|-------------|--------------|------|
| FITKOCCModelCoverEdges | FCOCCModelCoverEdges | ✅ 已移植 |
| FITKOCCModelClosedWireSurface | FCOCCModelClosedWireSurface | ✅ 已移植 |
| FITKOCCModelOffsetSurface | FCOCCModelOffsetSurface | ✅ 已移植 |
| FITKOCCModelExtrudeSurface | FCOCCModelExtrudeSurface | ✅ 已移植 |
| FITKOCCModelRevolSurface | FCOCCModelRevolSurface | ✅ 已移植 |
| FITKOCCModelSweepSurface | FCOCCModelSweepSurface | ✅ 已移植（BRepOffsetAPI_MakePipeShell） |
| FITKOCCModelMultiSectionSurface | FCOCCModelMultiSectionSurface | ✅ 已移植（BRepOffsetAPI_ThruSections） |
| FITKOCCModelBridgeSurface | FCOCCModelBridgeSurface | ✅ 已移植（BRepFill_Filling 四边填充） |
| FITKOCCModelSolidSurface | FCOCCModelSolidSurface | ✅ 已移植 |

### 2.4 面操作 (FITKOCCModelOperFace → FCOCCModelOperFace 等)

| 源类 (FITK) | FreeCAE 对应 | 状态 |
|-------------|--------------|------|
| FITKOCCModelOperFace（基类） | — | ❌ 未作为独立命令移植（FC 仅有子类命令） |
| FITKOCCModelOperFaceFillHoles | FCOCCModelOperFaceFillHoles | ✅ 已移植 |
| FITKOCCModelOperFaceFillGaps | FCOCCModelOperFaceFillGaps | ✅ 已移植 |
| FITKOCCModelOperFaceDeleteFloatingEdge | FCOCCModelOperFaceDeleteFloatingEdge | ✅ 已移植 |
| FITKOCCModelRemoveFace | FCOCCModelOperFaceRemoveFace | ✅ 已移植 |
| FITKOCCModelRepairSmallFaces | FCOCCModelRepairSmallFaces | ✅ 已移植（独立文件） |
| FITKOCCModelExtendFace | FCOCCModelExtendFace | ✅ 已移植（当前为桩实现） |
| FITKOCCModelReplaceFaces | FCOCCModelReplaceFaces | ✅ 已移植（独立文件） |

### 2.5 复合体 (FITKOCCModelOperCompound)

| 源类 (FITK) | FreeCAE 对应 | 状态 |
|-------------|--------------|------|
| FITKOCCModelOperCompound（基类） | 仅作基类，未单独注册 | FC 只注册 Add/Remove，与源一致可接受 |
| FITKOCCModelOperAddCompound | FCOCCModelOperAddCompound | ✅ 已移植 |
| FITKOCCModelOperRemoveCompound | FCOCCModelOperRemoveCompound | ✅ 已移植 |

---

## 三、已移植内容简要对照

以下在 FCGeometryCommand 中已有对应实现并在 FCOCCGeoCompRegister 中注册（或使用 FC 接口层）：

- **基类与注册**：FCAbstractOCCModel、OCCShapeAgent、FCOCCGeoCompRegister  
- **基本体 / 部件 / 点**：Box、Cylinder、Sphere、Part、Point、Import、Export、ImportFlattenAssembly  
- **布尔与多体**：OperBool、NBodyUnite  
- **实体**：ClosedSurfaceSolid、Extrude/Revol/Sweep/MultiSection Solid 与 Cut  
- **体特征**：Chamfer、Fillet、Defeature、RemoveChamfer、RemoveFillet、FillHole、CircularHole  
- **变换**：Transform、TransformByTwoPoints、TransformByDirAndDis、Rotate、Scale、Mirror、RectangularPattern、CircularPattern  
- **分割**：CurveSplitter、SurfaceSplitter、SolidSplitter  
- **边/面/体分块**：PartitionEdge（Parameter/Point/DatumPlane）、PartitionFace 全系列、PartitionSolid 全系列  
- **线/面操作**：OperLine、MiddleOperLine、RepairSmallEdges、RemoveFace、FillGaps、**FillHoles**、**ExtendFace**（桩）、DeleteFloatingEdge、RepairSmallFaces、ReplaceFaces、CoverEdges  
- **点派生**：CurveEnd、CurveEndDistance、CurveRadio、PointProjectionOnCurve、TwoCurveIntersectPoint、PointProjectionOnSurface、CurveIntersectSurface、InteractionPoint  
- **曲线**：Line、Segment、Wire、Circle、ThreePointsCircle、CircleArc、Ellipse、ThreePointsEllipse、EllipseArc、Hyperbola、ThreePointsHyperbola、HyperbolaArc、Parabola、ThreePointsParabola、ParabolaArc、BezierByControlPoints、BsplineByThroughPoints、TrimmedCurve（桩）、OffsetCurve、CurveProjectionOnSurface、SurfaceIntersectionSurface、BridgeCurve、SurfaceEdge  
- **曲面**：CoverEdges、ClosedWireSurface、OffsetSurface、ExtrudeSurface、RevolSurface、SweepSurface、MultiSectionSurface、BridgeSurface、SolidSurface  
- **其它**：Stitch、ExportTopos、GeoSketchDeleter、Stiffener、NearestPointLine、ExtractCenter、FromMesh、OperImprintSolid、OperAdd/RemoveCompound、**Sketch2D**（已移植）、**ImportedPart**、**SampleCurve**（已移植）  
- **基准**：FC 使用 FCGeometryInterface 的 FCGeoReferencePoint/Line/Plane 等，未移植 FITKOCCReference* OCC 实现  

---

## 四、建议移植优先级（按依赖与用途）

1. **高**（与现有体/面流程衔接紧）——**已完成**  
   - **FCOCCModelOperFaceFillHoles**、**FCOCCModelExtendFace**（已移植，ExtendFace 为桩）  
   - **FCOCCModelCurveEnd / CurveEndDistance / CurveRadio** 等点派生（已全部移植）  

2. **中**（扩展建模能力）——**部分完成**  
   - **FCOCCModelCurve**：Line/Segment/Wire/Circle/ThreePointsCircle/CircleArc/Ellipse 已移植；其余（双曲/抛物/样条/Offset/Trimmed 等）未移植  
   - **FCOCCModelSurface**：ClosedWireSurface 已移植；OffsetSurface、ExtrudeSurface、RevolSurface、SweepSurface 等未移植  
   - **FCOCCModelSketch2D**：已移植（当前为桩，输出空 Compound）  
   - **FCOCCModelImportedPart**：已移植  

3. **低**（辅助与可选）  
   - FITKOCCVirtualTopoCreator、FITKOCCShapeTriangulate 已移植进 FCAbstractOCCModel::buildVirtualTopo / triangulation  
   - FITKOCCGeomTools、FITKOCCAlgTools、FITKOCCComandCommon、FITKOCCFeature、FITKOCCExtendTool（按需抽取）  
   - FITKOCCDXFReader、FITKOCCModelRefine、FITKShapeFix_FixSmallFace（FITKOCCSampleCurve 已移植为 FCOCCSampleCurve）  

4. **基准**  
   - FITKOCCReferencePoint/Line/Plane：若需基准与 OCC 形状强绑定（如参数化、约束求解），再考虑做 FCOCC 版；当前用 FC 接口层即可。  

---

## 五、源与目标路径速查

| 项目 | 路径 |
|------|------|
| FITKGeoCompOCC 源码 | `ignoreFiles/AppFlow/FITK_Component/FITKGeoCompOCC/` |
| 项目文件 | `FITKGeoCompOCC.pri`（HEADERS + SOURCES 即完整文件列表） |
| 注册逻辑 | `FITKGeoCompOCCInterface.cpp` 构造函数 |
| FreeCAE 移植库 | `src/FCGeometryCommand/` |
| FreeCAE 注册 | `src/FCGeometryCommand/FCOCCGeoCompRegister.cpp` |

以上清单按当前仓库状态整理，后续若 FCGeometryInterface 或 FCGeometryCommand 增删枚举/类，需同步更新本表。
