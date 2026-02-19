/**
 * @file FCAbstractGlobalDataFactory.cpp
 * @brief 全局数据工厂抽象基类实现（移植自 APPFlow FITKAbstractGlobalDataFactory）
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#include "FCAbstractGlobalDataFactory.h"
#include "FCGlobalData.h"

namespace FC {

FCAbstractGlobalDataFactory::FCAbstractGlobalDataFactory()
{
    
}

void FCAbstractGlobalDataFactory::createGlobalData(FCGlobalData *globalData)
{
    //错误判断
    if (globalData == nullptr) return;
    //创建网格
    auto d = this->createMeshData();
    globalData->insertData(GDTMesh, d);
    //创建几何
    d = this->createGeoData();
    globalData->insertData(GDTGeom, d);
    //创建几何实体层（可选，应用层可重写 createGeometryEntityModel 从几何命令列表构建）
    d = this->createGeometryEntityModel(globalData);
    if (d) globalData->insertData(GDTGeomEntity, d);
    //创建物理数据
    d = this->createPhysicsData();
    globalData->insertData(GDTPhysics, d);
    //创建后处理数据
    d = this->createPostData();
    globalData->insertData(GDTPost, d);
    //创建软件运行时数据
    //         d = this->createRunTimeSetting();
    //         globalData->insertData(GDTRunTime, d);
    //创建其他数据
    QHash<int, FCAbstractDataObject*>  othersData = this->createOtherData();
    QList<int> dts = othersData.keys();
    for (auto dt : dts)
    {
        globalData->insertData(dt, othersData.value(dt));
    }
     
}

FCAbstractDataObject *FCAbstractGlobalDataFactory::createMeshData()
{
    return nullptr;
}

FCAbstractDataObject *FCAbstractGlobalDataFactory::createGeoData()
{
    return nullptr;
}

FCAbstractDataObject *FCAbstractGlobalDataFactory::createPhysicsData()
{
    return nullptr;
}

FCAbstractDataObject *FCAbstractGlobalDataFactory::createPostData()
{
    return nullptr;
}

QHash<int, FCAbstractDataObject *> FCAbstractGlobalDataFactory::createOtherData()
{
    return QHash<int, FCAbstractDataObject*>();
}

FCAbstractDataObject* FCAbstractGlobalDataFactory::createGeometryEntityModel(FCGlobalData*)
{
    return nullptr;
}

FCAbstractGlobalDataFactory::~FCAbstractGlobalDataFactory() = default;

} // namespace FC
