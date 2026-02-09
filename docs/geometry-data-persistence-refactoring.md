# FreeCAE几何数据持久化架构重构方案

## 一、当前架构问题分析

### 1.1 问题描述

当前FreeCAE的几何数据保存功能混杂在几何操作模块中，存在以下问题：

#### 问题1：职责不清
- **导出功能作为命令**：`FCGeoModelExport` 继承自 `FCAbsGeoCommand`，将数据保存作为几何操作命令
- **文件写入在ShapeAgent层**：`FCAbstractOCCModel::writeToFile()` 在形状代理层实现文件写入
- **命令调用文件写入**：`FCOCCModelExport::update()` 在命令的 `update()` 方法中调用文件写入

```cpp
// 当前实现：导出作为命令
class FCGeoModelExport : public FCAbsGeoCommand {
    bool update() override;  // 在命令中处理导出
};

// 当前实现：文件写入在ShapeAgent
class FCAbstractOCCModel : public FCAbsGeoShapeAgent {
    bool writeToFile(const QString& file);  // 在ShapeAgent中写入文件
};

// 当前实现：命令调用ShapeAgent的文件写入
bool FCOCCModelExport::update() {
    // ...
    return occModel->writeToFile(path);  // 命令调用ShapeAgent的方法
}
```

#### 问题2：耦合度高
- 几何操作与数据保存耦合：修改保存逻辑需要修改命令代码
- 格式支持分散：不同格式的保存逻辑分散在不同类中
- 难以统一管理：没有统一的数据持久化入口

#### 问题3：扩展性差
- 添加新格式需要在多个地方修改
- 无法统一处理序列化/反序列化
- 难以支持项目级别的数据保存

#### 问题4：序列化机制缺失
- `FCAbstractDataObject` 有 `serialize()` 和 `deserialize()` 方法，但实现为空
- 没有统一的序列化框架
- 无法支持项目文件的保存和加载

### 1.2 当前架构图

```
┌─────────────────────────────────────────────────────────┐
│              几何命令层 (Command Layer)                 │
│  ┌──────────────────────────────────────────┐           │
│  │  FCGeoModelExport (导出命令)             │           │
│  │  - update() 调用文件写入                 │           │
│  └──────────────────────────────────────────┘           │
│                          ↓                              │
│  ┌──────────────────────────────────────────┐           │
│  │  FCAbsGeoCommand (命令基类)             │           │
│  │  - 几何操作逻辑                         │           │
│  └──────────────────────────────────────────┘           │
└─────────────────────────────────────────────────────────┘
                          ↓
┌─────────────────────────────────────────────────────────┐
│              形状代理层 (Shape Agent Layer)             │
│  ┌──────────────────────────────────────────┐           │
│  │  FCAbstractOCCModel                     │           │
│  │  - writeToFile() 文件写入逻辑           │           │
│  │  - 格式判断 (BREP/STEP/IGES/STL)       │           │
│  └──────────────────────────────────────────┘           │
└─────────────────────────────────────────────────────────┘
                          ↓
┌─────────────────────────────────────────────────────────┐
│              数据对象层 (Data Object Layer)               │
│  ┌──────────────────────────────────────────┐           │
│  │  FCAbstractDataObject                    │           │
│  │  - serialize() 空实现                    │           │
│  │  - deserialize() 空实现                 │           │
│  └──────────────────────────────────────────┘           │
└─────────────────────────────────────────────────────────┘
```

**问题**：
- ❌ 数据保存逻辑分散在命令和ShapeAgent中
- ❌ 没有统一的数据持久化层
- ❌ 序列化机制不完善

## 二、重构目标

### 2.1 设计原则

1. **职责分离**：几何操作与数据持久化分离
2. **统一接口**：统一的数据持久化接口
3. **可扩展性**：易于添加新的保存格式
4. **向后兼容**：不影响现有功能

### 2.2 目标架构

```
┌─────────────────────────────────────────────────────────┐
│              几何命令层 (Command Layer)                 │
│  ┌──────────────────────────────────────────┐           │
│  │  FCAbsGeoCommand                        │           │
│  │  - 仅负责几何操作                       │           │
│  │  - 不包含数据保存逻辑                   │           │
│  └──────────────────────────────────────────┘           │
└─────────────────────────────────────────────────────────┘
                          ↓
┌─────────────────────────────────────────────────────────┐
│              数据持久化层 (Persistence Layer)             │
│  ┌──────────────────────────────────────────┐           │
│  │  FCGeometryPersistenceManager            │           │
│  │  - 统一的数据保存/加载入口               │           │
│  │  - 格式注册与管理                       │           │
│  └──────────────────────────────────────────┘           │
│  ┌──────────────────────────────────────────┐           │
│  │  FCGeometrySerializer (接口)            │           │
│  │  - serialize()                          │           │
│  │  - deserialize()                       │           │
│  └──────────────────────────────────────────┘           │
│                          ↓                              │
│  ┌──────────────┐ ┌──────────────┐ ┌──────────────┐    │
│  │ BREP格式     │ │ STEP格式    │ │ IGES格式    │    │
│  │ Serializer   │ │ Serializer  │ │ Serializer  │    │
│  └──────────────┘ └──────────────┘ └──────────────┘    │
└─────────────────────────────────────────────────────────┘
                          ↓
┌─────────────────────────────────────────────────────────┐
│              数据对象层 (Data Object Layer)              │
│  ┌──────────────────────────────────────────┐           │
│  │  FCAbstractDataObject                   │           │
│  │  - serialize() 完整实现                 │           │
│  │  - deserialize() 完整实现              │           │
│  └──────────────────────────────────────────┘           │
└─────────────────────────────────────────────────────────┘
```

## 三、重构方案

### 3.1 核心组件设计

#### 3.1.1 数据持久化管理器

```cpp
/**
 * @file FCGeometryPersistenceManager.h
 * @brief 几何数据持久化管理器
 */
class FCGEOMETRYINTERFACE_API FCGeometryPersistenceManager : public QObject
{
    Q_OBJECT
public:
    static FCGeometryPersistenceManager* instance();
    
    // 序列化器注册
    void registerSerializer(const QString& format, FCGeometrySerializer* serializer);
    void unregisterSerializer(const QString& format);
    
    // 数据保存
    bool saveGeometry(FCAbsGeoCommand* command, const QString& filePath);
    bool saveGeometry(FCAbsGeoShapeAgent* agent, const QString& filePath);
    bool saveGeometryList(const QList<FCAbsGeoCommand*>& commands, const QString& filePath);
    
    // 数据加载
    FCAbsGeoCommand* loadGeometry(const QString& filePath);
    QList<FCAbsGeoCommand*> loadGeometryList(const QString& filePath);
    
    // 格式支持查询
    QStringList getSupportedFormats() const;
    bool isFormatSupported(const QString& format) const;
    
    // 项目级别保存/加载
    bool saveProject(const QString& projectPath);
    bool loadProject(const QString& projectPath);
    
signals:
    void geometrySaved(FCAbsGeoCommand* command, const QString& filePath);
    void geometryLoaded(FCAbsGeoCommand* command, const QString& filePath);
    
private:
    QHash<QString, FCGeometrySerializer*> _serializers;
    QString _defaultFormat;
};
```

#### 3.1.2 序列化器接口

```cpp
/**
 * @file FCGeometrySerializer.h
 * @brief 几何数据序列化器接口
 */
class FCGEOMETRYINTERFACE_API FCGeometrySerializer : public QObject
{
    Q_OBJECT
public:
    explicit FCGeometrySerializer(QObject* parent = nullptr);
    virtual ~FCGeometrySerializer() = default;
    
    // 格式信息
    virtual QString formatName() const = 0;
    virtual QStringList fileExtensions() const = 0;
    virtual QString description() const = 0;
    
    // 序列化
    virtual bool serialize(FCAbsGeoCommand* command, const QString& filePath) = 0;
    virtual bool serialize(FCAbsGeoShapeAgent* agent, const QString& filePath) = 0;
    virtual bool serializeList(const QList<FCAbsGeoCommand*>& commands, const QString& filePath) = 0;
    
    // 反序列化
    virtual FCAbsGeoCommand* deserialize(const QString& filePath) = 0;
    virtual QList<FCAbsGeoCommand*> deserializeList(const QString& filePath) = 0;
    
    // 格式验证
    virtual bool canRead(const QString& filePath) const = 0;
    virtual bool canWrite(const QString& filePath) const = 0;
};
```

#### 3.1.3 BREP格式序列化器示例

```cpp
/**
 * @file FCGeometryBREPSerializer.h
 * @brief BREP格式序列化器
 */
class FCGEOMETRYINTERFACE_API FCGeometryBREPSerializer : public FCGeometrySerializer
{
    Q_OBJECT
public:
    explicit FCGeometryBREPSerializer(QObject* parent = nullptr);
    
    QString formatName() const override { return "BREP"; }
    QStringList fileExtensions() const override { return {"brep"}; }
    QString description() const override { return "OpenCASCADE BREP Format"; }
    
    bool serialize(FCAbsGeoCommand* command, const QString& filePath) override;
    bool serialize(FCAbsGeoShapeAgent* agent, const QString& filePath) override;
    bool serializeList(const QList<FCAbsGeoCommand*>& commands, const QString& filePath) override;
    
    FCAbsGeoCommand* deserialize(const QString& filePath) override;
    QList<FCAbsGeoCommand*> deserializeList(const QString& filePath) override;
    
    bool canRead(const QString& filePath) const override;
    bool canWrite(const QString& filePath) const override;
    
private:
    TopoDS_Shape* getShapeFromCommand(FCAbsGeoCommand* command);
    TopoDS_Shape* getShapeFromAgent(FCAbsGeoShapeAgent* agent);
};
```

#### 3.1.4 项目序列化器

```cpp
/**
 * @file FCGeometryProjectSerializer.h
 * @brief 项目文件序列化器（JSON/XML格式）
 */
class FCGEOMETRYINTERFACE_API FCGeometryProjectSerializer : public FCGeometrySerializer
{
    Q_OBJECT
public:
    explicit FCGeometryProjectSerializer(QObject* parent = nullptr);
    
    QString formatName() const override { return "FreeCAE Project"; }
    QStringList fileExtensions() const override { return {"fcae", "fcae.json"}; }
    QString description() const override { return "FreeCAE Project Format"; }
    
    bool serialize(FCAbsGeoCommand* command, const QString& filePath) override;
    bool serializeList(const QList<FCAbsGeoCommand*>& commands, const QString& filePath) override;
    
    FCAbsGeoCommand* deserialize(const QString& filePath) override;
    QList<FCAbsGeoCommand*> deserializeList(const QString& filePath) override;
    
    // 项目级别保存/加载
    bool saveProject(FCGeoCommandManager* manager, const QString& filePath);
    bool loadProject(FCGeoCommandManager* manager, const QString& filePath);
    
private:
    QJsonObject commandToJson(FCAbsGeoCommand* command);
    FCAbsGeoCommand* jsonToCommand(const QJsonObject& json);
};
```

### 3.2 数据对象序列化扩展

#### 3.2.1 扩展FCAbstractDataObject

```cpp
/**
 * @file FCAbstractDataObject.h (扩展)
 */
class FCGEOMETRYINTERFACE_API FCAbstractDataObject : public QObject
{
public:
    // 序列化（完整实现）
    virtual QString serialize(int label = -1);
    virtual bool deserialize(const QString& text, int label = -1);
    
    // JSON序列化（新增）
    virtual QJsonObject toJson() const;
    virtual bool fromJson(const QJsonObject& json);
    
    // 二进制序列化（新增）
    virtual QByteArray toBinary() const;
    virtual bool fromBinary(const QByteArray& data);
    
protected:
    // 子类重写实现具体序列化逻辑
    virtual QJsonObject serializeToJson() const;
    virtual bool deserializeFromJson(const QJsonObject& json);
};
```

#### 3.2.2 FCAbsGeoCommand序列化实现

```cpp
/**
 * @file FCAbsGeoCommand.cpp (扩展)
 */
QJsonObject FCAbsGeoCommand::serializeToJson() const
{
    QJsonObject json;
    
    // 基本信息
    json["type"] = static_cast<int>(getGeometryCommandType());
    json["id"] = getDataObjectID();
    json["name"] = getName();
    
    // 参数序列化
    QJsonObject paramsJson;
    QVariantMap params = getAllParams();
    for (auto it = params.begin(); it != params.end(); ++it) {
        paramsJson[it.key()] = QJsonValue::fromVariant(it.value());
    }
    json["params"] = paramsJson;
    
    // 虚拓扑序列化
    if (_shapeAgent) {
        QJsonObject shapeJson = _shapeAgent->serializeToJson();
        json["shape"] = shapeJson;
    }
    
    return json;
}

bool FCAbsGeoCommand::deserializeFromJson(const QJsonObject& json)
{
    // 基本信息
    setName(json["name"].toString());
    
    // 参数反序列化
    QJsonObject paramsJson = json["params"].toObject();
    for (auto it = paramsJson.begin(); it != paramsJson.end(); ++it) {
        setParam(it.key(), it.value().toVariant());
    }
    
    // 虚拓扑反序列化
    if (json.contains("shape")) {
        QJsonObject shapeJson = json["shape"].toObject();
        if (_shapeAgent) {
            _shapeAgent->deserializeFromJson(shapeJson);
        }
    }
    
    // 更新几何
    return update();
}
```

### 3.3 重构步骤

#### 阶段一：创建持久化框架（2-3周）

1. **创建核心接口**
   - `FCGeometrySerializer` 接口
   - `FCGeometryPersistenceManager` 管理器
   - 基础序列化器基类

2. **实现BREP序列化器**
   - 将 `FCAbstractOCCModel::writeToFile()` 逻辑迁移到序列化器
   - 实现BREP格式的序列化/反序列化

3. **注册机制**
   - 实现序列化器注册机制
   - 支持动态注册新格式

#### 阶段二：迁移现有功能（2-3周）

1. **重构导出命令**
   - `FCGeoModelExport` 改为调用 `FCGeometryPersistenceManager`
   - 移除 `FCAbstractOCCModel::writeToFile()` 方法
   - 保持向后兼容

2. **实现其他格式序列化器**
   - STEP格式序列化器
   - IGES格式序列化器
   - STL格式序列化器

3. **扩展数据对象序列化**
   - 实现 `FCAbstractDataObject::serialize()` 和 `deserialize()`
   - 实现JSON序列化支持

#### 阶段三：项目级别支持（2-3周）

1. **项目序列化器**
   - 实现 `FCGeometryProjectSerializer`
   - 支持项目文件的保存和加载
   - JSON格式存储

2. **命令管理器序列化**
   - `FCGeoCommandManager` 支持序列化
   - 保存/加载整个命令列表

3. **集成到项目系统**
   - 与 `FCProjectInterface` 集成
   - 支持项目文件的保存和加载

### 3.4 使用示例

#### 3.4.1 保存单个几何

```cpp
// 旧方式（需要移除）
FCGeoModelExport* exportCmd = new FCGeoModelExport();
exportCmd->setFileName("box.brep");
exportCmd->addExportCommandID(boxCmd->getDataObjectID());
exportCmd->update();

// 新方式
FCGeometryPersistenceManager* mgr = FCGeometryPersistenceManager::instance();
mgr->saveGeometry(boxCmd, "box.brep");
```

#### 3.4.2 保存多个几何

```cpp
QList<FCAbsGeoCommand*> commands;
commands << boxCmd << cylinderCmd << sphereCmd;

FCGeometryPersistenceManager* mgr = FCGeometryPersistenceManager::instance();
mgr->saveGeometryList(commands, "geometry.brep");
```

#### 3.4.3 加载几何

```cpp
FCGeometryPersistenceManager* mgr = FCGeometryPersistenceManager::instance();
FCAbsGeoCommand* cmd = mgr->loadGeometry("box.brep");
if (cmd) {
    FCGeoCommandManager* cmdMgr = FCGeoCommandManager::instance();
    cmdMgr->appendDataObj(cmd);
}
```

#### 3.4.4 保存项目

```cpp
FCGeometryPersistenceManager* mgr = FCGeometryPersistenceManager::instance();
mgr->saveProject("project.fcae");
```

#### 3.4.5 注册新格式

```cpp
// 注册自定义格式
FCGeometryCustomSerializer* customSerializer = new FCGeometryCustomSerializer();
FCGeometryPersistenceManager* mgr = FCGeometryPersistenceManager::instance();
mgr->registerSerializer("custom", customSerializer);
```

## 四、迁移策略

### 4.1 向后兼容

1. **保留旧接口（标记为废弃）**
   ```cpp
   // 标记为废弃，但保留实现
   [[deprecated("Use FCGeometryPersistenceManager::saveGeometry() instead")]]
   bool FCAbstractOCCModel::writeToFile(const QString& file) {
       // 内部调用新的持久化管理器
       FCGeometryPersistenceManager* mgr = FCGeometryPersistenceManager::instance();
       return mgr->saveGeometry(_command, file);
   }
   ```

2. **渐进式迁移**
   - 新代码使用新接口
   - 旧代码逐步迁移
   - 提供迁移工具

### 4.2 测试策略

1. **单元测试**
   - 序列化器单元测试
   - 持久化管理器测试
   - 格式兼容性测试

2. **集成测试**
   - 保存/加载流程测试
   - 项目文件测试
   - 多格式转换测试

3. **回归测试**
   - 确保现有功能不受影响
   - 验证向后兼容性

## 五、预期收益

### 5.1 架构改进

1. **职责清晰**
   - 几何操作与数据持久化分离
   - 每个组件职责单一

2. **可扩展性**
   - 易于添加新格式
   - 统一的扩展接口

3. **可维护性**
   - 代码组织更清晰
   - 易于理解和维护

### 5.2 功能提升

1. **统一接口**
   - 统一的数据保存/加载接口
   - 简化使用方式

2. **格式支持**
   - 易于添加新格式
   - 支持项目级别保存

3. **序列化机制**
   - 完整的序列化支持
   - 支持JSON/二进制格式

## 六、实施计划

### 6.1 时间估算

- **阶段一**：2-3周（持久化框架）
- **阶段二**：2-3周（迁移现有功能）
- **阶段三**：2-3周（项目级别支持）

**总计**：6-9周（1.5-2.5个月）

### 6.2 风险控制

1. **兼容性风险**
   - 保持向后兼容
   - 渐进式迁移

2. **性能风险**
   - 优化序列化性能
   - 支持增量保存

3. **质量风险**
   - 充分测试
   - 代码审查

---

**文档版本**：V1.0  
**创建日期**：2026-02-09  
**作者**：AI Assistant  
**审核状态**：待审核
