# FCGeometryEntity 模块使用示例

本文档给出 FCGeometryEntity 模块的两种典型用法：**函数式几何 DAG 建模**（FCGeometryModule）与 **几何实体模型构建**（FCGeometryEntityBuilder）。

---

## 1. 函数式几何 DAG 建模（FCGeometryModule）

用“操作 + 依赖”描述几何，不保存实体，只保存 DAG；通过 `build()` 或 `buildDirty()` 得到最终形状。执行器（FCGeometryNodeExecutor）由 OCC 等几何内核实现并注入。

### 示例：两个 Block 做 Union

```cpp
#include <FCGeometryEntity/FCGeometryModule.h>
#include <FCGeometryEntity/FCGeometryNodeExecutor.h>
#include <QMap>
#include <QVariant>

using namespace FC;

// 假设已有 OCC 实现的执行器（项目内由 FCGeometryCommand 等提供）
extern FCGeometryNodeExecutor* createOCCExecutor();

void exampleGeometryModule()
{
    FCGeometryModule module;

    // 1) 注入执行器（必须，否则 build 无效）
    FCGeometryNodeExecutor* executor = createOCCExecutor();
    module.setExecutor(executor);

    // 2) 添加两个 Block 节点
    FCGeoParamSet paramsA;
    paramsA["length"] = 10.0;
    paramsA["width"]  = 10.0;
    paramsA["height"] = 5.0;
    int idA = module.addBlock(paramsA, QStringLiteral("BlockA"));

    FCGeoParamSet paramsB;
    paramsB["length"] = 8.0;
    paramsB["width"]  = 8.0;
    paramsB["height"] = 6.0;
    int idB = module.addBlock(paramsB, QStringLiteral("BlockB"));

    // 3) 添加 Union 节点，依赖 idA、idB
    int idUnion = module.addUnion(idA, idB, QStringLiteral("UnionAB"));

    // 4) 全量构建，得到最终形状（QVariant，OCC 层可转为 TopoDS_Shape）
    QVariant result = module.build();
    if (result.isValid()) {
        // 使用 result：显示、导出或交给实体构建器等
        int lastId = module.lastOutputNodeId(); // 即 idUnion
    }

    // 5) 修改中间节点后仅重算 dirty（可选）
    module.updateNode(idA, paramsA);  // 例如改尺寸
    QVariant result2 = module.buildDirty();
}
```

### 参数约定（示例）

- **Block**：`length`, `width`, `height`
- **Cylinder**：如 `radius`, `height`
- **Sphere**：如 `radius`
- **Fillet**：如 `radius`，并需传入 `FCSelectionRule*` 指定要倒角的边/面

具体键名以实际 `FCGeometryNodeExecutor` 实现为准。

---

## 2. 几何实体模型构建（FCGeometryEntityBuilder）

从已有的几何命令列表（FCGeoCommandList）或全局几何组件管理器（FCGlobalGeoComponentManager）构建 **FCGeometryEntityModel**，得到稳定的 Domain/Boundary/Edge/Point ID 视图，供网格、物理、后处理使用。

### 示例：从全局数据构建实体模型

```cpp
#include <FCGeometryEntity/FCGeometryEntityBuilder.h>
#include <FCGeometryEntity/FCGeometryEntityModel.h>
#include <FCData/FCGlobalData.h>
// 假设 GDTGeom 为几何命令列表类型标识
// FCGlobalData::getData<FCGeoCommandList>(GDTGeom)

void exampleEntityBuilder(FC::FCGlobalData* globalData)
{
    if (!globalData) return;

    FC::FCGeoCommandList* geoList = globalData->getData<FC::FCGeoCommandList>(GDTGeom);
    FC::FCGeometryEntityBuilder builder;

    // 方式一：新建实体模型（调用方负责释放 model）
    FC::FCGeometryEntityModel* model = builder.build(geoList);
    if (model) {
        QList<int> domainIds = model->getDomainIds();
        QList<int> boundaryIds = model->getBoundaryIds();
        // 用于网格、物理选择等
        delete model;
    }
}
```

### 示例：重建已有实体模型

当几何命令列表更新后，在已有 FCGeometryEntityModel 上重建，避免重复创建对象（与 FCGlobalDataFactory::refreshGeometryEntityModel 一致）：

```cpp
void refreshEntityModel(FC::FCGlobalData* globalData)
{
    if (!globalData) return;

    FC::FCGeoCommandList* geoList = globalData->getData<FC::FCGeoCommandList>(GDTGeom);
    FC::FCGeometryEntityModel* entityModel = globalData->getData<FC::FCGeometryEntityModel>(GDTGeomEntity);
    if (!geoList || !entityModel) return;

    FC::FCGeometryEntityBuilder builder;
    bool ok = builder.rebuild(geoList, entityModel);
    if (!ok) {
        // 重建失败处理
    }
}
```

### 使用实体模型中的 ID

```cpp
// 获取某实体的源 member 索引（与 FCGlobalGeoComponent 的 member 对应）
int memberId = model->getSourceMemberId(FCGeometryEntityLevel::Domain, entityId);

// 层级关系（若已由 OCC 拓扑填充）
QList<int> boundaries = model->getBoundariesOfDomain(domainId);
QList<int> edges = model->getEdgesOfBoundary(boundaryId);

// 可选名称
QString name = model->getEntityName(FCGeometryEntityLevel::Boundary, boundaryId);
model->setEntityName(FCGeometryEntityLevel::Boundary, boundaryId, QStringLiteral("Face1"));
```

---

## 3. 两部分的衔接

- **FCGeometryModule**：负责“定义几何操作 DAG”和“求值得到形状”（QVariant）。若需要把该形状纳入全局几何数据（如 FCGeoCommandList / FCGlobalGeoComponentManager），需在应用层将 `build()` 的结果写入对应结构。
- **FCGeometryEntityBuilder**：只读已有几何数据（FCGeoCommandList / FCGlobalGeoComponentManager），生成或更新 **FCGeometryEntityModel**，不执行 DAG。

典型流程：用 FCGeometryModule 编辑并 build 得到形状 → 应用层把结果同步到 FCGeoCommandList/FCGlobalGeoComponentManager → 用 FCGeometryEntityBuilder 从该数据 build/rebuild FCGeometryEntityModel，供后续模块使用。
