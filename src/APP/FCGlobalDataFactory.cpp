/**
 * @file FCGlobalDataFactory.cpp
 * @brief 应用层全局数据工厂实现（COMSOL 式函数式几何：GDTGeom 使用 FCGeometryDAGData）
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#include "FCGlobalDataFactory.h"
#include <FCData/FCGlobalData.h>
#include <FCGeometryEntity/FCGeometryDAGData.h>
#include <FCGeometryEntity/FCGeometryEntityBuilder.h>
#include <FCGeometryEntity/FCGeometryEntityModel.h>
#include <FCGeometryCommand/FCOCCGeometryNodeExecutor.h>

namespace FC {

FCGlobalDataFactory::~FCGlobalDataFactory() = default;

FCAbstractDataObject* FCGlobalDataFactory::createGeoData()
{
    FCGeometryDAGData* dagData = new FCGeometryDAGData();
    m_geometryExecutor = std::make_unique<OCC::FCOCCGeometryNodeExecutor>();
    dagData->setExecutor(m_geometryExecutor.get());
    return dagData;
}

FCAbstractDataObject *FCGlobalDataFactory::createPhysicsData()
{
    
    return nullptr;
}

FCAbstractDataObject *FCGlobalDataFactory::createMeshData()
{
    return nullptr;
}

FCAbstractDataObject *FCGlobalDataFactory::createPostData()
{
    // 创建后处理数据
    return nullptr;
}

QHash<int, FCAbstractDataObject *> FCGlobalDataFactory::createOtherData()
{
    // 不创建其他数据
    return QHash<int, FCAbstractDataObject *>();
}

FCAbstractDataObject* FCGlobalDataFactory::createGeometryEntityModel(FCGlobalData* globalData)
{
    if (!globalData) return nullptr;
    FCGeometryDAGData* dagData = globalData->getData<FCGeometryDAGData>(GDTGeom);
    if (!dagData) return nullptr;
    dagData->ensureBuild();
    FCGeometryEntityBuilder builder;
    FCGeometryEntityModel* model = builder.buildFromGlobalGeoComponentManager(dagData->getGlobalGeoCompManager());
    return model;
}

void FCGlobalDataFactory::refreshGeometryEntityModel(FCGlobalData* globalData)
{
    if (!globalData) return;
    FCGeometryDAGData* dagData = globalData->getData<FCGeometryDAGData>(GDTGeom);
    FCGeometryEntityModel* entityModel = globalData->getData<FCGeometryEntityModel>(GDTGeomEntity);
    if (!dagData || !entityModel) return;
    dagData->ensureBuild();
    FCGeometryEntityBuilder builder;
    builder.rebuild(dagData->getGlobalGeoCompManager(), entityModel);
}

} // namespace FC
