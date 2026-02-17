/**
 * @file FCGlobalDataFactory.h
 * @brief 应用层全局数据工厂（移植自 APPFlow GlobalDataFactory）
 * @details 创建并持有单例 FCGlobalData，供主窗口/数据管理器获取几何、网格、求解等全局数据入口
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#ifndef FCGLOBALDATAFACTORY_H
#define FCGLOBALDATAFACTORY_H

#include <FCData/FCAbstractGlobalDataFactory.h>
#include <FCData/FCGlobalData.h>

namespace FC {

/**
 * @brief 应用层全局数据工厂
 *
 * 实现 FCAbstractGlobalDataFactory，首次调用 getGlobalData() 时创建 FCGlobalData
 * 并加入 FCDataRepo，后续返回同一实例。
 */
class FCGlobalDataFactory : public FCAbstractGlobalDataFactory
{
public:
    FCGlobalDataFactory() = default;
    ~FCGlobalDataFactory() override;
    
private:
        
    /**
     * @brief 创建全局几何，可以为空
     * @return Core::FITKAbstractDataObject*
     */
    virtual FCAbstractDataObject* createGeoData() override;
    
    /**
     * @brief 创建全局物理数据，一般不为空
     * @return FCAbstractDataObject*
     */
    virtual FCAbstractDataObject* createPhysicsData() override;
    
    /**
     * @brief 创建全局网格，可以为空
     * @return FCAbstractDataObject*
     */
    virtual FCAbstractDataObject* createMeshData() override;
    
    /**
     * @brief 创建后处理数据
     * @return FCAbstractDataObject*
     */
    virtual FCAbstractDataObject* createPostData() override;
    
    /**
     * @brief 创建其他数据，返回值哈希中的int至少为FITKGolbalData.h/FITKGlobalDT::GDTOther
     * @return QHash< int,Core::FITKAbstractDataObject*>
     */
    virtual QHash< int,FCAbstractDataObject*> createOtherData() override;


};

} // namespace FC

#endif // FCGLOBALDATAFACTORY_H
