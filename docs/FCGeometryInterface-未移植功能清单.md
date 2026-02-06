# FCGeometryInterface 未移植功能清单

本文档对比了 `FITKInterfaceGeometry` 和 `FCGeometryInterface`，列出尚未移植到 FC 的功能。

## 一、明确不移植的功能（按约定）

### 1. Python 绑定
- **PyFITKGeoBehavior** - Python 行为绑定
- **PyFITKSketch2D** - Python 草图绑定

**说明**：这些是 Python 扩展接口，FreeCAE 项目约定不移植 Python 绑定。

---

## 二、尚未移植的核心功能

### 1. CircularHole（圆孔）
- **FITK 类**：`FITKAbsGeoModelCircularHoleSolid`
- **FITK 枚举**：`FGTCircularHole`
- **功能**：在实体上创建圆孔，支持通过面和边定位
- **状态**：✅ 已移植
- **FC 类**：`FCAbsGeoModelCircularHoleSolid`
- **位置**：`src/FCGeometryInterface/FCAbsGeoModelOperSolid.h`

**接口特点**：
- 继承自 `FCAbsGeoModelSolid`
- 支持通过面（OperFace）和两条边（OperEdge1/OperEdge2）定位
- 支持距离定位（DistanceLocateHole1/2）
- 支持设置直径（Diameter）
- 支持翻转方向（Flip）

**对应关系**：
- FC 中 `FCAbsGeoModelOperSolid` 包含：Chamfer、Fillet、Defeature、RemoveChamfer、RemoveFillet、FillHole、**CircularHole** ✅

---

### 2. FITKAbsGeoFeature 基类
- **FITK 基类**：`FITKAbsGeoFeature`（抽象特征基类）
- **FITK 子类**：`FITKAbsGeoStiffener`（肋特征）
- **FC 对应**：`FCGeoStiffener`（直接继承 `FCAbsGeoCommand`）
- **状态**：⚠️ 部分移植（缺少基类抽象）

**说明**：
- FC 中 `FCGeoStiffener` 已移植，但直接继承自 `FCAbsGeoCommand`
- FITK 中有 `FITKAbsGeoFeature` 作为特征基类，FC 中缺少此抽象层
- 如果未来需要添加其他特征类型，可能需要补充基类

---

### 3. FillHolesFace（填补孔洞-面）
- **FITK 枚举**：`FGTFillHolesFace`
- **FITK 类**：`FITKAbsGeoModelOperFace` 中的 FillHoles 操作
- **FC 枚举**：`FCGeoEnum::FGTFillHolesFace` ✅ 已存在
- **FC 类**：需要确认 `FCGeoModelOperFace` 中是否有对应实现
- **状态**：⚠️ 枚举已移植，实现待确认

**说明**：
- FC 枚举中已有 `FGTFillHolesFace`
- 需要检查 `FCGeoModelOperFace` 是否实现了对应的功能类

---

## 三、可选扩展功能（接口层已具备基类，子类可按需补充）

### 1. 曲线子类扩展
**已移植的曲线子类**：
- ✅ Line（直线）
- ✅ Segment（线段）
- ✅ Wire（线串）
- ✅ Circle（圆）
- ✅ ThreePointsCircle（三点圆）
- ✅ CircleArc（圆弧）
- ✅ Ellipse（椭圆）
- ✅ ThreePointsEllipse（三点椭圆）
- ✅ EllipseArc（椭圆弧）
- ✅ Hyperbola（双曲线）
- ✅ ThreePointsHyperbola（三点双曲线）
- ✅ HyperbolaArc（双曲线弧）
- ✅ Parabola（抛物线）
- ✅ ThreePointsParabola（三点抛物线）
- ✅ ParabolaArc（抛物线弧）
- ✅ BezierByControlPoints（贝塞尔-控制点）
- ✅ BsplineByThroughPoints（B样条-通过点）
- ✅ TrimmedCurve（裁剪曲线）
- ✅ OffsetCurve（偏移曲线）
- ✅ CurveProjectionOnSurface（曲线投影到面）
- ✅ SurfaceIntersectionSurface（面面交线）
- ✅ BridgeCurve（桥接曲线）
- ✅ SurfaceEdge（面的边线）

**说明**：曲线基类 `FCGeoModelCurve` 已移植，主要子类已覆盖。如需更多子类，可按需添加。

---

### 2. 曲面子类扩展
**已移植的曲面子类**：
- ✅ ClosedWireSurface（封闭线面）
- ✅ OffsetSurface（偏移曲面）
- ✅ ExtrudeSurface（拉伸曲面）
- ✅ RevolSurface（旋转曲面）
- ✅ SweepSurface（扫略曲面）
- ✅ MultiSectionSurface（多截面曲面）
- ✅ BridgeSurface（桥接曲面）
- ✅ SolidSurface（体的面）

**说明**：曲面基类 `FCGeoModelSurface` 已移植，主要子类已覆盖。如需更多子类，可按需添加。

---

## 四、总结

### 已移植功能（核心框架）
✅ **核心框架**：命令基类、形状代理、虚拓扑、基准、工厂、部件/实例  
✅ **基础体**：Box、Cylinder、Sphere  
✅ **删除**：Delete、MultiDelete  
✅ **布尔运算**：Bool、NBodyUnite  
✅ **导入导出**：Import、Export、ExportTopos  
✅ **变换**：Transformation 及所有子类  
✅ **点/线/面/体**：Point、Curve、Surface、Solid 及主要子类  
✅ **切割**：Cut 及所有子类  
✅ **体操作**：Chamfer、Fillet、Defeature、RemoveChamfer、RemoveFillet、FillHole、CircularHole  
✅ **参考几何**：ReferencePoint、ReferenceLine、ReferencePlane 及子类  
✅ **分割/缝合**：Splitter、Stitch  
✅ **分区**：PartitionEdge、PartitionFace、PartitionSolid 及子类  
✅ **草图**：Sketch2D、SketchFillet、SketchDeleter  
✅ **构造**：Construction  
✅ **修复/替换**：RepairSmallEdges、RepairSmallFaces、ReplaceFaces、CoverEdges  
✅ **虚拓扑组合**：VirtualTopoCombine  
✅ **其他**：ExtractCenter、FromMesh、NearestPointLine、SampleCurve  
✅ **辅助工具**：ExtendTool、VirtualTopoMapper、AbstractGeomTools、CommandList、GlobalGeoComponent、DatumList、GeometryMeshEntity、ImportFileThread、ExportFileThread

### 未移植功能
✅ **CircularHole**：圆孔功能（已移植）  
⚠️ **FITKAbsGeoFeature 基类**：特征基类抽象（可选，FCGeoStiffener 已直接继承 FCAbsGeoCommand）  
⚠️ **FillHolesFace 实现**：填补孔洞-面功能实现（枚举已存在，实现待确认）

### 不移植功能
🚫 **Python 绑定**：PyFITKGeoBehavior、PyFITKSketch2D

---

## 五、建议

1. ✅ **CircularHole 已移植**：圆孔功能已完成移植
2. **确认 FillHolesFace**：检查 `FCAbsGeoModelOperFace` 是否已实现对应功能
3. **特征基类**：如果未来需要添加更多特征类型，可考虑补充 `FCAbsGeoFeature` 基类（当前 FCGeoStiffener 直接继承 FCAbsGeoCommand）
4. **曲线/曲面扩展**：当前已覆盖主要子类，如需更多可按需添加

---

*最后更新：2026-02-06*  
*对比范围：`ignoreFiles/AppFlow/FITK_Interface/FITKInterfaceGeometry` ↔ `src/FCGeometryInterface`*
