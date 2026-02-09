# FITKGeoCompOCC 移植说明

## 一、FITK 源码位置

FITKGeoCompOCC 完整源码位于（被 .gitignore 忽略，仅本地可见）：

```
ignoreFiles/AppFlow/FITK_Component/FITKGeoCompOCC/
```

该目录是 **FITK 的整块 OCC 几何实现模块**，包含：

- **FITKGeoCompOCCInterface**：向 `FITKInterfaceGeometryFactory` 注册所有 OCC 命令/基准/GeomTools 的“组件接口”（构造函数里 `insertCommandCreateFun` / `insertDatumCreateFun`）。
- **FITKAbstractOCCModel**：OCC 形状代理基类（继承 `FITKAbsGeoShapeAgent`），实现 `getShapeCount`、`getShape(type,id)`、`updateShape`、`buildVirtualTopo`、`triangulation`、`getBoundaryBox` 等。
- **OCCShapeAgent**：混入类，持有 `FITKAbstractOCCModel* _occShapeAgent`，各 FITKOCC* 命令类继承“接口命令 + OCCShapeAgent”。
- **FITKOCC***：各类 OCC 命令实现（ModelSolid、ModelCut、ModelOperSolid、VirtualTopoCreator、ReferencePoint/Line/Plane 等），见 `FITKGeoCompOCC.pri` 中的 SOURCES/HEADERS。

## 二、FreeCAE 中已移植内容（FCGeometryCommand）

| FITK 源 | FreeCAE 对应 | 说明 |
|--------|--------------|------|
| 全局几何分组从 OCC 形状填充 | **FCOCCGeoComponentBuilder** | 从 `TopoDS_Shape` 遍历 Face/Edge/Solid，填充 `FCGlobalGeoComponentManager`（与 FITK 虚拓扑索引思路一致，member 为 1-based 探索序号）。 |
| FITKAbstractOCCModel | **FCAbstractOCCModel** | 当前工程中由 FCGeometryCommand 头文件引用，若尚未实现，可对照 `FITKAbstractOCCModel.h/cpp` 移植。 |
| FITKOCCModel*（部分） | **FCOCCModelSolid / FCOCCModelCut / FCOCCModelOperSolid / FCOCCModelCircularHole** | 已有 OCC 命令类，继承 FC 接口 + OCCShapeAgent。 |
| FITKGeoCompOCCInterface 注册逻辑 | **未移植** | 需在应用层或本模块提供“注册函数”，将 FCOCC* 命令创建器注册到 `FCGeoInterfaceFactory`，参考 `FITKGeoCompOCCInterface.cpp` 构造函数。 |

## 三、后续移植可参考的 FITK 文件

- **OCC 抽象基类与代理**：`FITKAbstractOCCModel.h`、`FITKAbstractOCCModel.cpp`
- **工厂注册**：`FITKGeoCompOCCInterface.cpp`（`insertCommandCreateFun` / `insertDatumCreateFun` 列表）
- **虚拓扑**：`FITKOCCVirtualTopoCreator.*`、`FITKOCCVirtualTopoMapper`（对应 FC 的虚拓扑与几何分组）
- **索引与类型映射**：FITK 使用 `TopTools_IndexedMapOfShape` + `TopExp::MapShapes`，子形状 ID 从 1 开始，与 `FCOCCGeoComponentBuilder` 当前 1-based 序号一致

按“移植而非拷贝”的原则，新代码应使用 FC 命名空间（`FC::`）、`FCGeoInterfaceFactory`、`FCAbsGeoShapeAgent`、`FCGlobalGeoComponent` 等已有接口，并依赖 FCData、FCGeometryInterface、FCModelInterface，不再依赖 FITKCore / FITKInterface* / FITKAppFramework。
