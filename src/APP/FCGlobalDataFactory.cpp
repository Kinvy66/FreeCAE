/**
 * @file FCGlobalDataFactory.cpp
 * @brief 应用层全局数据工厂实现（移植自 APPFlow GlobalDataFactory）
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#include "FCGlobalDataFactory.h"

namespace FC {

FCGlobalDataFactory::~FCGlobalDataFactory()
{
    
}

FCAbstractDataObject *FCGlobalDataFactory::createGeoData()
{
    return nullptr;
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


} // namespace FC
