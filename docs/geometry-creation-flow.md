# FreeCAE几何体创建流程分析

## 一、完整流程概览

创建几何体的流程从用户点击UI按钮开始，经过多个层次的处理，最终生成几何实体。以下是完整的流程：

```
用户点击UI按钮
    ↓
QAction::triggered 信号
    ↓
FCActionEventHandler::execOperator()
    ↓
FCOperatorRepo::getOperator() - 获取操作器
    ↓
FCActionOperator::actionTriggered()
    ↓
FCActionCreateCubeOperator::execProfession()
    ↓
FCGeoInterfaceFactory::createCommandT() - 创建命令
    ↓
FCOCCModelBox (OCC实现) - 创建具体命令对象
    ↓
设置参数 (setPoint1, setLength)
    ↓
命令::update() - 更新几何
    ↓
FCOCCModelBox::update() - OCC实现
    ↓
BRepPrimAPI_MakeBox - OCC创建Box
    ↓
FCAbstractOCCModel::updateShape() - 更新形状
    ↓
buildVirtualTopo() - 构建虚拓扑
    ↓
triangulation() - 三角化
    ↓
FCGeoCommandList::appendDataObj() - 添加到命令列表
    ↓
完成
```

## 二、详细流程分析

### 2.1 阶段一：UI触发（用户交互层）

#### 1.1 用户点击按钮

```cpp
// 用户在UI中点击"创建立方体"按钮
// UI层：FCAppRibbonArea 或类似UI组件
QAction* createCubeAction = ...;  // 已创建的QAction
```

#### 1.2 QAction信号触发

```cpp
// QAction发出triggered信号
emit createCubeAction->triggered();
```

#### 1.3 信号连接到事件处理器

```cpp
// FCAppController::initConnection()
void FCAppController::initConnection()
{
    QList<QAction*> actionList = mActions->getAllActions();
    for (QAction* action : actionList) {
        // 所有Action的triggered信号都连接到execOperator
        connect(action, &QAction::triggered, 
                mActionHandler, &FCActionEventHandler::execOperator);
    }
}
```

**关键点**：
- 所有Action统一连接到 `FCActionEventHandler::execOperator()`
- 通过Action的 `objectName()` 来识别操作类型

### 2.2 阶段二：操作器查找（操作器层）

#### 2.1 事件处理器接收信号

```cpp
// FCActionEventHandler::execOperator()
void FCActionEventHandler::execOperator()
{
    QObject* obj = this->sender();  // 获取发送信号的对象
    QAction* action = qobject_cast<QAction*>(obj);
    QString objName = action->objectName();  // 例如："actionCreateCUbe"
    
    // 从操作器仓库获取操作器
    FCActionOperator* actOper = this->getOperator(action);
    if (actOper != nullptr) {
        actOper->setEmitter(action);
        actOper->actionTriggered();  // 执行操作
    }
}
```

#### 2.2 从仓库获取操作器

```cpp
// FCActionEventHandler::getOperator()
FCActionOperator* FCActionEventHandler::getOperator(QObject* object)
{
    QAction* action = qobject_cast<QAction*>(object);
    QString objName = action->objectName();  // "actionCreateCUbe"
    
    // 从操作器仓库获取
    FCOperatorRepo* repo = FCOPERATORREPO;
    FCActionOperator* oper = repo->getOperatorT<FCActionOperator>(objName);
    return oper;
}
```

**关键点**：
- 操作器通过 `Register2FCOperatorRepo` 宏自动注册
- 注册键为 `"actionCreateCUbe"`（与Action的objectName一致）

#### 2.3 操作器注册机制

```cpp
// FCActionCreateCubeOperator.h
// 使用宏自动注册操作器
Register2FCOperatorRepo(actionCreateCUbe, FCActionCreateCubeOperator);

// 这个宏会在静态初始化时自动注册到FCOperatorRepo
```

### 2.3 阶段三：操作执行（操作器层）

#### 3.1 操作器执行流程

```cpp
// FCActionOperator::actionTriggered()
bool FCActionOperator::actionTriggered()
{
    this->preArgs();              // 1. 预处理参数
    bool OK = this->execProfession();  // 2. 执行业务逻辑
    if (!OK) return false;
    return this->execGUI();       // 3. 执行UI相关操作
}
```

**执行顺序**：
1. `preArgs()` - 参数预处理（可选）
2. `execProfession()` - **核心业务逻辑**（创建几何体）
3. `execGUI()` - UI相关操作（可选）

#### 3.2 业务逻辑执行

```cpp
// FCActionCreateCubeOperator::execProfession()
bool FCActionCreateCubeOperator::execProfession()
{
    // 步骤1：获取几何接口工厂
    FCGeoInterfaceFactory* factory = FCGeoInterfaceFactory::instance();
    
    // 步骤2：通过工厂创建Box命令
    FCGeoModelBox* boxCmd = factory->createCommandT<FCGeoModelBox>(FCGeoEnum::FGTBox);
    
    // 步骤3：设置参数
    double point1[3] = { 0.0, 0.0, 0.0 };
    double length[3] = { 100.0, 100.0, 100.0 };
    boxCmd->setPoint1(point1);
    boxCmd->setLength(length);
    
    // 步骤4：设置名称
    QString name = QString("Box_%1").arg(boxCmd->getDataObjectID());
    boxCmd->setName(name);
    
    // 步骤5：更新几何（关键步骤）
    if (!boxCmd->update()) {
        delete boxCmd;
        return false;
    }
    
    // 步骤6：添加到几何命令列表
    FCGeoCommandList* geoList = FCDATAREPO->getFirstDataByType<FCGeoCommandList>();
    geoList->appendDataObj(boxCmd);
    
    return true;
}
```

### 2.4 阶段四：命令创建（工厂层）

#### 4.1 工厂创建命令

```cpp
// FCGeoInterfaceFactory::createCommandT()
template<class T> 
T* FCGeoInterfaceFactory::createCommandT(FCGeoEnum::FCGeometryComType t) 
{
    FCAbsGeoCommand* c = createCommand(t);  // 调用工厂函数
    T* ct = dynamic_cast<T*>(c);
    if (!ct && c) delete c;
    return ct;
}

FCAbsGeoCommand* FCGeoInterfaceFactory::createCommand(FCGeoEnum::FCGeometryComType t)
{
    if (!_interfaceFuns.contains(t)) return nullptr;
    GeoCommandFuns fun = _interfaceFuns.value(t);  // 获取创建函数
    if (!fun) return nullptr;
    return fun();  // 调用函数创建命令对象
}
```

#### 4.2 OCC命令注册

```cpp
// FCOCCGeoCompRegister.cpp
// 在程序启动时注册OCC实现
void registerOCCGeometryCommands()
{
    FC::FCGeoInterfaceFactory* fac = FC::FCGeoInterfaceFactory::instance();
    
    // 注册Box命令的创建函数
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTBox, 
                                []() { return new FCOCCModelBox; });
    // ... 其他命令注册
}
```

**关键点**：
- 工厂模式：通过类型枚举获取创建函数
- OCC实现：`FCOCCModelBox` 是 `FCGeoModelBox` 的OCC实现
- 多态：接口层 `FCGeoModelBox`，实现层 `FCOCCModelBox`

#### 4.3 OCC命令对象创建

```cpp
// FCOCCModelBox 构造函数
FCOCCModelBox::FCOCCModelBox()
    : OCCShapeAgent(this)  // 混入OCCShapeAgent
{
    _shapeAgent = _occShapeAgent;  // 设置ShapeAgent
}

// OCCShapeAgent 构造函数
OCCShapeAgent::OCCShapeAgent(FC::FCAbsGeoCommand* c)
{
    _occShapeAgent = new FCAbstractOCCModel(c);  // 创建OCC模型
}
```

**关键点**：
- `FCOCCModelBox` 继承自 `FCGeoModelBox`（接口）和 `OCCShapeAgent`（混入）
- 创建时自动创建 `FCAbstractOCCModel` 作为ShapeAgent
- ShapeAgent负责管理OCC的 `TopoDS_Shape`

### 2.5 阶段五：几何生成（命令层）

#### 5.1 命令更新

```cpp
// FCOCCModelBox::update() - OCC实现
bool FCOCCModelBox::update()
{
    // 步骤1：获取参数
    double p1[3], len[3];
    getPoint1(p1);      // 从接口层获取参数
    getLength(len);
    
    // 步骤2：转换为OCC几何类型
    gp_Pnt pt1(p1[0], p1[1], p1[2]);
    gp_Pnt pt2(p1[0] + len[0], p1[1] + len[1], p1[2] + len[2]);
    
    // 步骤3：使用OCC创建Box
    BRepPrimAPI_MakeBox cmd(pt1, pt2);
    cmd.Build();
    if (!cmd.IsDone()) return false;
    
    // 步骤4：获取生成的形状
    const TopoDS_Shape& out = cmd.Shape();
    if (out.IsNull()) return false;
    
    // 步骤5：更新ShapeAgent
    _occShapeAgent->updateShape(out);
    
    return true;
}
```

#### 5.2 ShapeAgent更新形状

```cpp
// FCAbstractOCCModel::updateShape()
void FCAbstractOCCModel::updateShape(const TopoDS_Shape& shape, bool buildVTopo)
{
    if (_shape && !_shape->IsNull() && !shape.IsNull() && shape.IsSame(*_shape))
        return;  // 形状未变化，跳过
    
    // 步骤1：更新形状
    *_shape = shape;
    _hashCode = shapeHashCode(shape);
    
    // 步骤2：清除旧的虚拓扑和网格
    if (_vtmanager && buildVTopo) _vtmanager->clear();
    if (_meshVS && buildVTopo) _meshVS->clear();
    
    if (_shape->IsNull()) return;
    
    // 步骤3：构建虚拓扑和三角化
    if (buildVTopo) {
        buildVirtualTopo(false);  // 构建虚拓扑
        triangulation();           // 三角化（用于可视化）
    }
}
```

**关键点**：
- `updateShape()` 会更新OCC的 `TopoDS_Shape`
- 自动构建虚拓扑（Virtual Topology）
- 自动三角化（用于3D可视化）

### 2.6 阶段六：虚拓扑构建（虚拓扑层）

#### 6.1 虚拓扑构建

```cpp
// FCAbstractOCCModel::buildVirtualTopo()
void FCAbstractOCCModel::buildVirtualTopo(bool keepTopos)
{
    if (_buildingTopo || !_vtmanager) return;
    _vtmanager->clear();
    // 完整虚拓扑创建可后续移植 FITKOCCVirtualTopoCreator
    // 目前是空实现，后续会实现完整的虚拓扑构建逻辑
}
```

**虚拓扑的作用**：
- 为几何实体分配拓扑ID（VirtualTopoId）
- 便于后续选择、载荷施加和网格关联
- 支持几何实体的逻辑分组

### 2.7 阶段七：添加到命令列表（数据管理层）

#### 7.1 添加到命令列表

```cpp
// FCActionCreateCubeOperator::execProfession()
// 获取几何命令列表
FCGeoCommandList* geoList = FCDATAREPO->getFirstDataByType<FCGeoCommandList>();
if (!geoList) {
    delete boxCmd;
    return false;
}

// 添加到几何命令列表
geoList->appendDataObj(boxCmd);
```

#### 7.2 数据对象注册

```cpp
// FCAbstractDataObject 构造函数
FCAbstractDataObject::FCAbstractDataObject(int parentDataID)
{
    _absDataID = FCDataRepo::instance()->getNextValidID();  // 分配ID
    _parentDataID = parentDataID;
    FCDataRepo::instance()->addDataObj(this);  // 注册到数据仓库
}
```

**关键点**：
- 每个命令对象自动分配唯一ID
- 自动注册到 `FCDataRepo`（全局数据仓库）
- 添加到 `FCGeoCommandList`（几何命令列表）

## 三、架构层次总结

### 3.1 层次结构

```
┌─────────────────────────────────────────────────────────┐
│  1. UI层 (User Interface Layer)                        │
│     - QAction (UI按钮)                                  │
│     - FCAppRibbonArea (Ribbon界面)                      │
└─────────────────────────────────────────────────────────┘
                          ↓
┌─────────────────────────────────────────────────────────┐
│  2. 事件处理层 (Event Handler Layer)                    │
│     - FCActionEventHandler                               │
│     - 信号槽连接                                         │
└─────────────────────────────────────────────────────────┘
                          ↓
┌─────────────────────────────────────────────────────────┐
│  3. 操作器层 (Operator Layer)                            │
│     - FCActionOperator (操作器基类)                     │
│     - FCActionCreateCubeOperator (创建立方体操作器)     │
│     - FCOperatorRepo (操作器仓库)                       │
└─────────────────────────────────────────────────────────┘
                          ↓
┌─────────────────────────────────────────────────────────┐
│  4. 工厂层 (Factory Layer)                              │
│     - FCGeoInterfaceFactory (几何命令工厂)              │
│     - 命令创建函数注册                                   │
└─────────────────────────────────────────────────────────┘
                          ↓
┌─────────────────────────────────────────────────────────┐
│  5. 命令层 (Command Layer)                              │
│     - FCAbsGeoCommand (命令基类)                        │
│     - FCGeoModelBox (Box命令接口)                       │
│     - FCOCCModelBox (Box命令OCC实现)                    │
└─────────────────────────────────────────────────────────┘
                          ↓
┌─────────────────────────────────────────────────────────┐
│  6. 形状代理层 (Shape Agent Layer)                       │
│     - FCAbsGeoShapeAgent (形状代理基类)                 │
│     - FCAbstractOCCModel (OCC形状代理)                  │
│     - TopoDS_Shape (OCC几何形状)                        │
└─────────────────────────────────────────────────────────┘
                          ↓
┌─────────────────────────────────────────────────────────┐
│  7. 虚拓扑层 (Virtual Topology Layer)                   │
│     - FCVirtualTopoManager (虚拓扑管理器)               │
│     - FCAbsVirtualTopo (虚拓扑对象)                     │
│     - VirtualTopoId (拓扑ID)                            │
└─────────────────────────────────────────────────────────┘
                          ↓
┌─────────────────────────────────────────────────────────┐
│  8. 数据管理层 (Data Management Layer)                   │
│     - FCDataRepo (数据仓库)                             │
│     - FCGeoCommandList (命令列表)                        │
│     - FCAbstractDataObject (数据对象基类)              │
└─────────────────────────────────────────────────────────┘
```

### 3.2 关键组件职责

| 组件 | 职责 | 关键方法 |
|------|------|---------|
| **QAction** | UI按钮，触发操作 | `triggered()` 信号 |
| **FCActionEventHandler** | 事件分发，查找操作器 | `execOperator()` |
| **FCOperatorRepo** | 操作器仓库，管理操作器 | `getOperatorT()` |
| **FCActionOperator** | 操作器基类，执行流程控制 | `actionTriggered()` |
| **FCGeoInterfaceFactory** | 命令工厂，创建几何命令 | `createCommandT()` |
| **FCAbsGeoCommand** | 命令基类，几何操作接口 | `update()` |
| **FCAbsGeoShapeAgent** | 形状代理，管理几何形状 | `updateShape()` |
| **FCVirtualTopoManager** | 虚拓扑管理，分配拓扑ID | `buildVirtualTopo()` |
| **FCGeoCommandList** | 命令列表，管理所有命令 | `appendDataObj()` |

## 四、数据流

### 4.1 参数流

```
用户输入/默认值
    ↓
FCActionCreateCubeOperator::execProfession()
    ↓
boxCmd->setPoint1() / setLength()
    ↓
FCGeoModelBox::_point1 / _lengthParam (存储)
    ↓
FCOCCModelBox::update() -> getPoint1() / getLength()
    ↓
BRepPrimAPI_MakeBox (OCC创建)
    ↓
TopoDS_Shape (OCC几何形状)
```

### 4.2 对象创建流

```
FCGeoInterfaceFactory::createCommandT(FGTBox)
    ↓
工厂函数: []() { return new FCOCCModelBox; }
    ↓
new FCOCCModelBox()
    ↓
构造函数: OCCShapeAgent(this)
    ↓
new FCAbstractOCCModel(command)
    ↓
new TopoDS_Shape()
    ↓
new FCVirtualTopoManager()
    ↓
对象创建完成
```

### 4.3 几何生成流

```
boxCmd->update()
    ↓
FCOCCModelBox::update()
    ↓
BRepPrimAPI_MakeBox::Build()
    ↓
TopoDS_Shape (生成)
    ↓
_occShapeAgent->updateShape(shape)
    ↓
buildVirtualTopo() (构建虚拓扑)
    ↓
triangulation() (三角化)
    ↓
几何生成完成
```

## 五、关键设计模式

### 5.1 工厂模式（Factory Pattern）

```cpp
// 工厂创建命令
FCGeoInterfaceFactory* factory = FCGeoInterfaceFactory::instance();
FCGeoModelBox* boxCmd = factory->createCommandT<FCGeoModelBox>(FGTBox);
```

**优点**：
- 解耦命令创建和使用
- 支持多种几何引擎（OCC/Parasolid等）
- 易于扩展新命令类型

### 5.2 命令模式（Command Pattern）

```cpp
// 命令封装几何操作
FCAbsGeoCommand* cmd = ...;
cmd->update();  // 执行几何操作
```

**优点**：
- 支持撤销/重做
- 命令可以序列化
- 命令可以组合

### 5.3 代理模式（Agent Pattern）

```cpp
// ShapeAgent代理几何形状
FCAbsGeoShapeAgent* agent = cmd->getShapeAgent();
agent->updateShape(shape);
```

**优点**：
- 隐藏底层几何引擎细节
- 支持多几何引擎
- 统一接口

### 5.4 仓库模式（Repository Pattern）

```cpp
// 操作器仓库
FCOperatorRepo* repo = FCOPERATORREPO;
FCActionOperator* oper = repo->getOperatorT<FCActionOperator>(name);
```

**优点**：
- 统一管理操作器
- 支持动态注册
- 易于查找和使用

## 六、总结

### 6.1 流程特点

1. **分层清晰**：UI → 事件 → 操作器 → 工厂 → 命令 → ShapeAgent → 虚拓扑 → 数据管理
2. **职责单一**：每个层次职责明确，互不干扰
3. **易于扩展**：通过工厂和仓库模式，易于添加新功能
4. **多态支持**：接口层和实现层分离，支持多几何引擎

### 6.2 关键步骤

1. **用户交互**：点击UI按钮
2. **事件分发**：信号槽机制分发到操作器
3. **操作器执行**：操作器执行业务逻辑
4. **命令创建**：工厂创建几何命令
5. **几何生成**：命令调用OCC生成几何
6. **虚拓扑构建**：为几何分配拓扑ID
7. **数据管理**：添加到命令列表

### 6.3 设计优势

- ✅ **解耦**：各层次职责清晰，耦合度低
- ✅ **可扩展**：易于添加新命令、新操作器
- ✅ **可维护**：代码组织清晰，易于理解
- ✅ **可测试**：各层次可独立测试

---

**文档版本**：V1.0  
**创建日期**：2026-02-09  
**作者**：AI Assistant  
**审核状态**：待审核
