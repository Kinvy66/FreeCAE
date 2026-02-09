# FreeCAE函数式几何模块架构设计

## 一、架构对比

### 1.1 当前架构（对象式）

```
用户操作
    ↓
FCAbsGeoCommand (命令对象)
    ↓
参数设置 (简单值: 100.0, "box1")
    ↓
Command.update()
    ↓
FCAbsGeoShapeAgent (形状代理)
    ↓
生成/更新 TopoDS_Shape (OCC几何)
    ↓
构建虚拓扑 (VirtualTopo, ID: 1, 2, 3...)
    ↓
三角化与可视化
```

**特点**：
- 每个几何实体是独立对象
- 有固定的拓扑ID（VirtualTopoId）
- 参数是简单值
- 参数变化需要重新生成整个几何

### 1.2 目标架构（函数式）

```
用户操作
    ↓
FCAbsGeoCommand (命令对象)
    ↓
参数设置 (表达式: "param_length", "2*radius", "sin(t)*10")
    ↓
FCParameterManager (参数管理器)
    ↓
表达式解析与计算 (muParser/ExprTK)
    ↓
参数依赖图 (DAG) - 检测依赖关系
    ↓
Command.update() - 函数式计算
    ↓
FCAbsGeoShapeAgent (形状代理)
    ↓
生成/更新 TopoDS_Shape (参数化几何)
    ↓
构建虚拓扑 (VirtualTopo, ID保持稳定)
    ↓
三角化与可视化
```

**特点**：
- 几何由参数和表达式定义
- 参数变化时自动重新计算
- 支持参数依赖和嵌套
- 拓扑ID在更新周期内保持稳定

## 二、混合架构设计（推荐）

### 2.1 架构层次

```
┌─────────────────────────────────────────────────────────┐
│                   用户界面层 (UI Layer)                  │
│  - 参数输入界面                                          │
│  - 表达式编辑器                                          │
│  - 参数依赖可视化                                        │
└─────────────────────────────────────────────────────────┘
                          ↓
┌─────────────────────────────────────────────────────────┐
│                 命令层 (Command Layer)                   │
│  ┌──────────────────┐  ┌──────────────────┐            │
│  │ 对象式命令       │  │ 函数式命令       │            │
│  │ (现有)           │  │ (新增)           │            │
│  │ - FCGeoModelBox  │  │ - FCFunctionalBox│            │
│  │ - 简单参数       │  │ - 表达式参数     │            │
│  └──────────────────┘  └──────────────────┘            │
│                          ↓                              │
│  ┌──────────────────────────────────────────┐           │
│  │      FCAbsGeoCommand (基类)              │           │
│  │      - update()                          │           │
│  │      - getShapeAgent()                  │           │
│  └──────────────────────────────────────────┘           │
└─────────────────────────────────────────────────────────┘
                          ↓
┌─────────────────────────────────────────────────────────┐
│              参数管理层 (Parameter Layer)                │
│  ┌──────────────────────────────────────────┐           │
│  │      FCParameterManager                  │           │
│  │      - 参数注册与管理                    │           │
│  │      - 参数依赖图 (DAG)                  │           │
│  │      - 参数变化通知                      │           │
│  └──────────────────────────────────────────┘           │
│  ┌──────────────────────────────────────────┐           │
│  │      FCExpressionEngine                  │           │
│  │      - 表达式解析 (muParser/ExprTK)      │           │
│  │      - 表达式计算                        │           │
│  │      - 参数绑定                          │           │
│  └──────────────────────────────────────────┘           │
└─────────────────────────────────────────────────────────┘
                          ↓
┌─────────────────────────────────────────────────────────┐
│              形状代理层 (Shape Agent Layer)              │
│  ┌──────────────────────────────────────────┐           │
│  │      FCAbsGeoShapeAgent (基类)           │           │
│  │      - updateShape()                     │           │
│  │      - buildVirtualTopo()               │           │
│  │      - triangulation()                  │           │
│  └──────────────────────────────────────────┘           │
│                          ↓                              │
│  ┌──────────────────────────────────────────┐           │
│  │      FCAbstractOCCModel (OCC实现)        │           │
│  │      - TopoDS_Shape 生成                 │           │
│  │      - 参数化几何创建                    │           │
│  └──────────────────────────────────────────┘           │
└─────────────────────────────────────────────────────────┘
                          ↓
┌─────────────────────────────────────────────────────────┐
│              虚拓扑层 (Virtual Topology Layer)           │
│  ┌──────────────────────────────────────────┐           │
│  │      FCVirtualTopoManager                │           │
│  │      - 虚拓扑管理                        │           │
│  │      - 拓扑ID分配                        │           │
│  │      - 拓扑引用管理                      │           │
│  └──────────────────────────────────────────┘           │
└─────────────────────────────────────────────────────────┘
```

### 2.2 核心类设计

#### 2.2.1 参数管理器

```cpp
class FCGEOMETRYINTERFACE_API FCParameterManager : public QObject
{
    Q_OBJECT
public:
    static FCParameterManager* instance();
    
    // 参数注册与管理
    bool registerParameter(const QString& name, double value);
    bool setParameterValue(const QString& name, double value);
    double getParameterValue(const QString& name) const;
    bool hasParameter(const QString& name) const;
    
    // 表达式支持
    bool registerExpression(const QString& name, const QString& expression);
    double evaluateExpression(const QString& expression) const;
    
    // 依赖管理
    void addDependency(const QString& param, const QString& dependsOn);
    QList<QString> getDependents(const QString& param) const;
    
signals:
    void parameterChanged(const QString& name, double oldValue, double newValue);
    void expressionChanged(const QString& name);
    
private:
    QHash<QString, double> _parameters;
    QHash<QString, QString> _expressions;
    // 依赖图 (DAG)
    QHash<QString, QSet<QString>> _dependencyGraph;
    QHash<QString, QSet<QString>> _reverseDependencyGraph;
};
```

#### 2.2.2 表达式引擎

```cpp
class FCGEOMETRYINTERFACE_API FCExpressionEngine
{
public:
    static FCExpressionEngine* instance();
    
    // 表达式解析与计算
    bool parseExpression(const QString& expression);
    double evaluate(const QHash<QString, double>& variables) const;
    
    // 参数提取
    QSet<QString> extractParameters(const QString& expression) const;
    
    // 表达式验证
    bool validateExpression(const QString& expression, QString* error = nullptr) const;
    
private:
    // 使用 muParser 或 ExprTK 作为底层引擎
    void* _parser;  // muParser 或 ExprTK 实例
};
```

#### 2.2.3 函数式命令基类

```cpp
class FCGEOMETRYINTERFACE_API FCFunctionalGeoCommand : public FCAbsGeoCommand
{
    Q_OBJECT
public:
    explicit FCFunctionalGeoCommand();
    
    // 表达式参数设置
    void setParameterExpression(const QString& paramName, const QString& expression);
    QString getParameterExpression(const QString& paramName) const;
    
    // 参数值获取（计算后的值）
    double getParameterValue(const QString& paramName) const;
    
    // 参数依赖
    void addParameterDependency(const QString& param, const QString& dependsOn);
    
protected:
    // 重写update，实现函数式计算
    bool update() override;
    
    // 参数计算
    virtual void computeParameters();
    
    // 参数变化处理
    virtual void onParameterChanged(const QString& name);
    
private:
    QHash<QString, QString> _parameterExpressions;  // 参数名 -> 表达式
    QHash<QString, double> _parameterValues;         // 参数名 -> 计算值
    QSet<QString> _parameterDependencies;           // 参数依赖集合
};
```

#### 2.2.4 函数式Box命令示例

```cpp
class FCGEOMETRYINTERFACE_API FCFunctionalBox : public FCFunctionalGeoCommand
{
public:
    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override {
        return FCGeoEnum::FGTFunctionalBox;
    }
    
    // 设置表达式参数
    void setLengthExpression(const QString& expression) {
        setParameterExpression("length", expression);
    }
    
    void setWidthExpression(const QString& expression) {
        setParameterExpression("width", expression);
    }
    
    void setHeightExpression(const QString& expression) {
        setParameterExpression("height", expression);
    }
    
    // 获取计算后的值
    double getLength() const { return getParameterValue("length"); }
    double getWidth() const { return getParameterValue("width"); }
    double getHeight() const { return getParameterValue("height"); }
    
protected:
    bool update() override {
        // 1. 计算参数值
        computeParameters();
        
        // 2. 获取OCC实现
        OCC::FCAbstractOCCModel* occModel = getTShapeAgent<OCC::FCAbstractOCCModel>();
        if (!occModel) return false;
        
        // 3. 创建参数化几何
        double length = getLength();
        double width = getWidth();
        double height = getHeight();
        
        // 4. 使用OCC创建Box
        BRepPrimAPI_MakeBox boxMaker(length, width, height);
        TopoDS_Shape shape = boxMaker.Shape();
        
        // 5. 更新形状
        occModel->updateShape(shape);
        
        return true;
    }
};
```

## 三、参数依赖管理

### 3.1 依赖图示例

```
参数定义：
- param_length = 100.0
- param_width = "param_length / 2"      // 依赖 param_length
- param_height = "param_length * 0.8"   // 依赖 param_length
- param_volume = "param_length * param_width * param_height"  // 依赖三个参数

依赖图：
param_length
    ├── param_width
    ├── param_height
    └── param_volume (间接依赖)
```

### 3.2 参数变化传播

```
1. 用户修改 param_length = 200.0
   ↓
2. FCParameterManager 检测到变化
   ↓
3. 查找依赖 param_length 的参数
   - param_width
   - param_height
   - param_volume
   ↓
4. 标记这些参数需要重新计算
   ↓
5. 通知使用这些参数的几何命令
   ↓
6. 命令自动调用 update() 重新计算几何
```

## 四、更新机制

### 4.1 更新流程

```
参数变化
    ↓
FCParameterManager::setParameterValue()
    ↓
检测依赖关系 (DAG遍历)
    ↓
标记需要更新的参数和命令
    ↓
发送参数变化信号
    ↓
FCFunctionalGeoCommand::onParameterChanged()
    ↓
检查参数是否实际变化
    ↓
如果变化，调用 update()
    ↓
computeParameters() - 重新计算参数值
    ↓
生成新的几何 (OCC)
    ↓
updateShape() - 更新形状
    ↓
buildVirtualTopo() - 重建虚拓扑
    ↓
triangulation() - 重新三角化
    ↓
可视化更新
```

### 4.2 增量更新优化

```cpp
class FCFunctionalGeoCommand
{
protected:
    // 参数缓存
    QHash<QString, double> _cachedParameterValues;
    
    bool update() override {
        // 1. 计算参数值
        computeParameters();
        
        // 2. 检查参数是否变化
        bool parametersChanged = false;
        for (auto it = _parameterValues.begin(); it != _parameterValues.end(); ++it) {
            if (_cachedParameterValues.value(it.key()) != it.value()) {
                parametersChanged = true;
                break;
            }
        }
        
        // 3. 如果参数未变化，跳过更新
        if (!parametersChanged) {
            return true;
        }
        
        // 4. 更新缓存
        _cachedParameterValues = _parameterValues;
        
        // 5. 执行几何更新
        // ...
    }
};
```

## 五、与现有架构的集成

### 5.1 兼容性设计

1. **命令基类扩展**
   - `FCAbsGeoCommand`保持不变
   - `FCFunctionalGeoCommand`继承`FCAbsGeoCommand`
   - 现有命令继续使用，不受影响

2. **参数系统扩展**
   - `FCVariantParams`扩展支持表达式
   - 简单值参数继续支持
   - 表达式参数作为新功能

3. **ShapeAgent兼容**
   - `FCAbsGeoShapeAgent`接口不变
   - 函数式命令使用相同的ShapeAgent
   - 更新逻辑封装在命令层

### 5.2 迁移路径

```
阶段1：基础扩展
- 添加 FCParameterManager
- 添加 FCExpressionEngine
- 扩展 FCVariantParams 支持表达式

阶段2：函数式命令
- 实现 FCFunctionalGeoCommand 基类
- 实现几个示例命令（Box、Cylinder等）
- 测试参数依赖和更新机制

阶段3：全面支持
- 扩展所有命令支持函数式参数
- 优化性能和用户体验
- 完善文档和示例
```

## 六、性能优化策略

### 6.1 缓存机制

```cpp
class FCParameterCache
{
public:
    // 表达式缓存
    struct ExpressionCache {
        QString expression;
        QHash<QString, double> variables;  // 参数值快照
        double result;
        bool isValid;
    };
    
    // 检查缓存是否有效
    bool isCacheValid(const QString& expression, 
                      const QHash<QString, double>& variables) const;
    
    // 获取缓存结果
    double getCachedResult(const QString& expression) const;
    
    // 更新缓存
    void updateCache(const QString& expression,
                     const QHash<QString, double>& variables,
                     double result);
    
private:
    QHash<QString, ExpressionCache> _expressionCache;
};
```

### 6.2 批量更新

```cpp
class FCParameterManager
{
public:
    // 批量设置参数
    void beginBatchUpdate();
    void setParameterValue(const QString& name, double value);
    void endBatchUpdate();  // 批量更新结束时统一通知
    
private:
    bool _inBatchUpdate;
    QSet<QString> _changedParameters;
};
```

## 七、总结

### 7.1 设计原则

1. **向后兼容**：现有功能不受影响
2. **渐进式扩展**：分阶段实施，逐步完善
3. **性能优先**：通过缓存和增量更新优化性能
4. **用户友好**：提供清晰的API和文档

### 7.2 关键优势

1. **灵活性**：支持复杂的参数化几何
2. **自动化**：参数变化时自动更新
3. **兼容性**：与现有架构良好兼容
4. **可扩展**：易于扩展新功能

---

**文档版本**：V1.0  
**创建日期**：2026-02-09  
**作者**：AI Assistant
