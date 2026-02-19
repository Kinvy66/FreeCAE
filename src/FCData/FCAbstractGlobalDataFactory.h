/**
 * @file FCAbstractGlobalDataFactory.h
 * @brief 全局数据工厂抽象基类（移植自 APPFlow FITKAbstractGlobalDataFactory）
 * @details 由应用层实现，创建或返回当前会话的 FCGlobalData
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#ifndef FCABSTRACTGLOBALDATAFACTORY_H
#define FCABSTRACTGLOBALDATAFACTORY_H

#include "FCDataAPI.h"

namespace FC {

class FCGlobalData;
class FCAbstractDataObject;

/**
 * @brief 全局数据工厂抽象接口
 *
 * 应用层（如 FCGlobalDataFactory）实现此类，负责创建或返回全局数据对象，
 * 便于统一获取几何命令列表、网格、求解参数等运行时数据的入口。
 */
class FCDATA_API FCAbstractGlobalDataFactory
{
public:
    FCAbstractGlobalDataFactory();// = default;
    virtual ~FCAbstractGlobalDataFactory() = 0;

    /**
     * @brief 执行创建操作，将数据存入全局数据对象
     * @return
     */
    virtual void createGlobalData(FCGlobalData* globalData);

    /** 获取当前全局数据（若尚未创建则先 createGlobalData） */
    // virtual FCGlobalData* getGlobalData() = 0;

private:
    
    /**
     * @brief 创建全局几何，可以为空
     * @return Core::FITKAbstractDataObject*
     */
    virtual FCAbstractDataObject* createGeoData();

    /**
     * @brief 创建全局物理数据，一般不为空
     * @return FCAbstractDataObject*
     */
    virtual FCAbstractDataObject* createPhysicsData();
    
    /**
     * @brief 创建全局网格，可以为空
     * @return FCAbstractDataObject*
     */
    virtual FCAbstractDataObject* createMeshData();

    /**
     * @brief 创建后处理数据
     * @return FCAbstractDataObject*
     */
    virtual FCAbstractDataObject* createPostData();

    /**
     * @brief 创建其他数据，返回值哈希中的int至少为FITKGolbalData.h/FITKGlobalDT::GDTOther
     * @return QHash< int,Core::FITKAbstractDataObject*>
     */
    virtual QHash< int,FCAbstractDataObject*> createOtherData();

    /**
     * @brief 创建几何实体层（Domain/Boundary/Edge/Point），由 FCGeometryEntity 构建
     * @param globalData 当前全局数据，用于取 GDTGeom 得到 FCGeoCommandList
     * @return 新建的 FCGeometryEntityModel 或 nullptr（默认不创建）
     */
    virtual FCAbstractDataObject* createGeometryEntityModel(FCGlobalData* globalData);

};

} // namespace FC

#endif // FCABSTRACTGLOBALDATAFACTORY_H
