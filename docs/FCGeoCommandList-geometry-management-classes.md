# FCGeoCommandList 几何管理涉及类的关系与功能

本文档说明以 **FCGeoCommandList** 为核心的几何管理相关类：FCGeoCommandManager、FCAbstractDataManager、FCAbstractDataManagerHelper、FCAbsGeoCommand 等的关系与各自职责。

---

## 一、类关系总览

```
FCAbstractDataObject (FCData)           ← 数据对象基类
        △
        │ 继承
        │
FCGeoCommandList  ────────────────────── 几何命令列表（入口）
        │
        ├── 继承 FCGeoCommandManager ──── 即“撤销列表”，管理 FCAbsGeoCommand*
        │         │
        │         └── 继承 FCAbstractDataManager<FCAbsGeoCommand>
        │                   │
        │                   └── 持有 FCAbstractDataManagerHelper + _dataList
        │
        ├── 持有 m_redoList: FCGeoCommandManager*   （重做列表）
        ├── 持有 m_datumMgr: FCDatumList*           （基准）
        └── 持有 m_globalGeoCompMgr: FCGlobalGeoComponentManager*  （全局几何分组）

FCAbsGeoCommand (单条命令)
        △
        │ 继承；且 部分命令 同时是 FCGeoCommandManager（树形子命令）
        │
FCAbsGeoPart / FCGeoSketch2D 等  ─────── 既是命令，又可包含子命令
```

- **FCGeoCommandList**：对外入口，既“是”一个命令管理器（撤销列表），又“持有”重做列表、基准、全局分组。
- **FCGeoCommandManager**：专门管理 `FCAbsGeoCommand*` 的容器，实现“列表 + 撤销/重做”所需的增删查。
- **FCAbstractDataManager\<T\>**：通用“数据管理器”模板，内部把具体操作委托给 **FCAbstractDataManagerHelper** 和 `_dataList`。
- **FCAbstractDataManagerHelper**：对 `QList<FCAbstractDataObject*>` 的增删改查、按 ID/名称/索引访问、重名检查等，不关心具体类型，被 FCAbstractDataManager 使用。

---

## 二、各类的功能

### 1. FCAbstractDataManagerPrivate（FCData）

- **文件**：`FCData/FCAbstractDataManagerPrivate.h`
- **角色**：抽象接口，定义“数据管理器”的契约（append/remove/getDataCount/getDataByIndex/checkName 等）。
- **不持有数据**：仅声明接口，由子类实现并持有实际列表。

---

### 2. FCAbstractDataManagerHelper（FCData）

- **文件**：`FCData/FCAbstractDataManagerHelper.h/cpp`
- **角色**：对**某一列 FCAbstractDataObject*** 做具体增删改查的辅助类，不关心元素具体类型（用基类指针）。
- **功能**：
  - 持有引用：`QList<FCAbstractDataObject*>& _dataList`（列表由外部传入，Helper 不拥有）。
  - 增删：appendData、insertDataObj、removeDataObj、removeDataObjWithoutRelease、removeByIndex/ByID/ByName、clear。
  - 查询：getDataCount、getDataByIndex、getDataByID、getDataByName、getDataListByName、getDataIndex、getIndexByID、getIDByIndex。
  - 其它：checkName（重名时自动加后缀）、setParentDataID/getParentDataID、sortObject、isUsedDataObject。
  - 与对象生命周期：connectHelper 连接 FCAbstractDataObject::dataObjectDestoried，在对象被销毁时从列表中移除（dataObjectDestoried）。
- **与 FCAbstractDataManager 关系**：FCAbstractDataManager 内部创建 Helper，并把自身的 `_dataList` 传给 Helper；所有对列表的操作都通过 Helper 完成，并加互斥锁。

---

### 3. FCAbstractDataManager\<T\>（FCData）

- **文件**：`FCData/FCAbstractDataManager.hpp`
- **角色**：泛型“数据管理器”，继承 FCAbstractDataManagerPrivate，**管理 T* 的列表**（T 需继承 FCAbstractDataObject）。
- **实现方式**：
  - 持有 `QList<FCAbstractDataObject*> _dataList` 和 `FCAbstractDataManagerHelper* _helper`，构造时 `_helper = new FCAbstractDataManagerHelper(_dataList)`。
  - 对外的 append/remove/getDataCount 等实现：加锁后转调 `_helper` 的对应方法。
  - 类型相关：用 `dynamic_cast<T*>` 做 isT/toT，保证只加入/返回 T 类型；getDataByIndex/getDataByID/getDataByName 返回 `T*`。
- **在几何里的用法**：`FCGeoCommandManager` 即 `FCAbstractDataManager<FCAbsGeoCommand>`，所以“命令列表”的存储和按索引/ID/名称访问，都来自这套模板 + Helper。

---

### 4. FCGeoCommandManager（FCGeometryInterface）

- **文件**：`FCGeometryInterface/FCAbsGeoCommand.h`（类定义）、`FCAbsGeoCommand.cpp`（部分实现）
- **定义**：`class FCGeoCommandManager : public FCAbstractDataManager<FCAbsGeoCommand>`
- **角色**：专门管理 **FCAbsGeoCommand*** 的容器，即“一堆几何命令”的列表；是 FCGeoCommandList 的“撤销列表”的实现基础。
- **功能**：
  - 继承自 FCAbstractDataManager 的：appendDataObj、removeDataObj、getDataCount、getDataByIndex、getDataByID、checkName 等（内部都走 Helper + _dataList）。
  - 扩展：getDataByID 递归到子管理器（若子项也是 FCGeoCommandManager，如 Part/Sketch）；getRootCommands() 返回根命令列表（基类返回空，FCGeoCommandList 自己实现 getRootCommandList 逻辑）。
- **与 FCGeoCommandList 关系**：FCGeoCommandList **继承** FCGeoCommandManager，因此“当前撤销列表”就是 FCGeoCommandList 自身；再单独持有一个 FCGeoCommandManager* 作为重做列表 m_redoList。

---

### 5. FCAbsGeoCommand（FCGeometryInterface）

- **文件**：`FCGeometryInterface/FCAbsGeoCommand.h/cpp`
- **角色**：单条几何命令的抽象基类（如 Box、Boolean、Extrude 等），代表一个可执行、可撤销/重做的操作。
- **功能**：
  - 命令类型、状态、参数：getGeometryCommandType、getCommandProp、setCommandStatus/getCommandStatus。
  - 几何结果：getShapeAgent（FCAbsGeoShapeAgent*）、getVirtualTopoManager、getVirtualTopo、getShapeT 等（与 OCC 形状/虚拓扑对接）。
  - 撤销/重做：undo()、redo()（基类默认 false，子类实现）。
  - 树形结构：部分派生类（如 FCAbsGeoPart、FCGeoSketch2D）**同时继承 FCGeoCommandManager**，即一个命令本身也可以包含子命令列表。
- **与 FCGeoCommandManager 关系**：被 FCGeoCommandManager 管理；FCGeoCommandList 的 getRootCommandList() 从本列表中筛出“根命令”（无被引用或 GUI 有效等）。

---

### 6. FCGeoCommandList（FCGeometryInterface）

- **文件**：`FCGeometryInterface/FCGeoCommandList.h/cpp`
- **角色**：几何命令列表的**总入口**，对应“当前文档的几何”：
  - 既是**撤销列表**（继承 FCGeoCommandManager，自身就是那串 FCAbsGeoCommand*）。
  - 又**持有**：重做列表、基准、全局几何分组。
- **组成**：
  - **撤销列表**：通过继承 FCGeoCommandManager 实现，即 appendDataObj 时加入的是“当前已执行命令”；undo 时从本列表移出并放入 m_redoList，redo 时相反。
  - **m_redoList**：FCGeoCommandManager*，重做栈。
  - **m_datumMgr**：FCDatumList*，基准（面、轴等）。
  - **m_globalGeoCompMgr**：FCGlobalGeoComponentManager*，按类型（Solid/Surface/Edge/Point）的全局几何分组，由 OCC 命令 Build 时填充。
- **常用 API**：
  - getRootCommandList()：当前“根命令”列表（用于显示树、参与 Build）。
  - getGlobalGeoCompManager()：供 FCGeometryEntityBuilder 等读取，构建实体视图。
  - canUndo/canRedo、undo()、redo()：若当前在草图则转调草图，否则操作本列表与 m_redoList。
  - appendDataObj(obj)：新命令加入时清空 m_redoList，再加入本列表（继承自 FCGeoCommandManager 的列表）。

---

## 三、数据流简图（谁持有谁）

```
FCGeoCommandList
├── [自身即] FCGeoCommandManager（撤销列表）
│   └── FCAbstractDataManager<FCAbsGeoCommand>
│       ├── _dataList: QList<FCAbstractDataObject*>
│       └── _helper: FCAbstractDataManagerHelper(_dataList)
│           └── 增删改查、checkName、dataObjectDestoried 等
├── m_redoList: FCGeoCommandManager*   （重做列表，结构同上）
├── m_datumMgr: FCDatumList*
└── m_globalGeoCompMgr: FCGlobalGeoComponentManager*
```

- **FCAbstractDataManagerHelper** 只认 `QList<FCAbstractDataObject*>&`，不关心是“命令”还是别的；**FCAbstractDataManager\<FCAbsGeoCommand>** 负责类型安全（只收/返回 FCAbsGeoCommand*）。
- **FCGeoCommandManager** 在这之上加了“命令”语义（如 getDataByID 递归、getRootCommands）；**FCGeoCommandList** 再在顶层加上重做列表、基准、全局分组和 undo/redo 流程。

---

## 四、总结表

| 类 | 模块 | 功能 |
|----|------|------|
| FCAbstractDataManagerPrivate | FCData | 数据管理器的抽象接口（append/remove/get 等）。 |
| FCAbstractDataManagerHelper | FCData | 对一列 FCAbstractDataObject* 的增删改查、重名检查、排序等；持有对外部 _dataList 的引用。 |
| FCAbstractDataManager\<T\> | FCData | 泛型数据管理器，持有 _dataList 和 _helper，加锁后委托 Helper，并做 T* 类型转换。 |
| FCGeoCommandManager | FCGeometryInterface | FCAbstractDataManager<FCAbsGeoCommand>，管理 FCAbsGeoCommand* 列表；支持按 ID 递归查找、根命令。 |
| FCAbsGeoCommand | FCGeometryInterface | 单条几何命令抽象；部分子类（Part/Sketch）同时是 FCGeoCommandManager，形成命令树。 |
| FCGeoCommandList | FCGeometryInterface | 几何总入口：继承 FCGeoCommandManager（撤销列表），持有重做列表、基准、全局分组；提供 undo/redo、getRootCommandList、getGlobalGeoCompManager 等。 |

因此：**FCGeoCommandList** 是几何管理的入口；“命令列表”的存储与访问由 **FCGeoCommandManager → FCAbstractDataManager → FCAbstractDataManagerHelper + _dataList** 完成；**FCAbstractDataManagerHelper** 是底层列表操作的实现者，不直接出现在几何接口中，但所有“按索引/ID/名称管理对象”的行为都来自它。
