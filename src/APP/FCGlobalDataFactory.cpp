/**
 * @file FCGlobalDataFactory.cpp
 * @brief 应用层全局数据工厂实现（移植自 APPFlow GlobalDataFactory）
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#include "FCGlobalDataFactory.h"
#include <FCData/FCGlobalData.h>
#include <FCGeometryInterface/FCGeoCommandList.h>
#include <FCGeometryEntity/FCGeometryEntityBuilder.h>
#include <FCGeometryEntity/FCGeometryEntityModel.h>

namespace FC {

FCGlobalDataFactory::~FCGlobalDataFactory()
{
    
}

FCAbstractDataObject *FCGlobalDataFactory::createGeoData()
{
    return new FCGeoCommandList();
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
    FCGeoCommandList* geoList = globalData->getData<FCGeoCommandList>(GDTGeom);
    FCGeometryEntityBuilder builder;
    FCGeometryEntityModel* model = builder.build(geoList);
    return model;
}

void FCGlobalDataFactory::refreshGeometryEntityModel(FCGlobalData* globalData)
{
    if (!globalData) return;
    FCGeoCommandList* geoList = globalData->getData<FCGeoCommandList>(GDTGeom);
    FCGeometryEntityModel* entityModel = globalData->getData<FCGeometryEntityModel>(GDTGeomEntity);
    if (!geoList || !entityModel) return;
    FCGeometryEntityBuilder builder;
    builder.rebuild(geoList, entityModel);
}

} // namespace FC
