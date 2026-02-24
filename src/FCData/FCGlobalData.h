
/**
 * @file FCGlobalData.h
 * @brief 全局运行数据容器
 * @date 2026-02-16
 * @version V0.0.1
 * @details
 * @copyright Copyright (c) 2026. All rights reserved.
 */
#ifndef FCGLOBALDATA_H
#define FCGLOBALDATA_H

#include "FCDataAPI.h"
#include "FCAbstractDataObject.h"
#include "FCAbstractGlobalDataFactory.h"
#include "FCMacros.h"
#include <QList>
#include <QMutexLocker>

namespace FC {

enum  FCGlobalDT
{
    GDTNone = 0,
    GDTMesh,
    GDTGeom,
    /** 几何实体层（Domain/Boundary/Edge/Point），由 FCGeometryEntity 模块构建 */
    GDTGeomEntity,
    GDTPhysics,
    GDTPost, 
    GDTOther = 1000,
    GDTEND = 3000,
};
class FCDATA_API FCGlobalData : public QObject
{
    friend FCAbstractGlobalDataFactory;
    Q_OBJECT
public:
    explicit FCGlobalData();
    virtual ~FCGlobalData();
    
    /**
     * @brief 获取物理数据并进行类型转换
     * @return T*
     */
    template<class T>
    T* getGeometryData()
    {
        return dynamic_cast<T*>(_gData.value(GDTGeom));
    }
    
    
    /**
     * @brief 获取物理数据并进行类型转换
     * @return T*
     */
    template<class T>
    T* getPhysicsData()
    {
        return dynamic_cast<T*>(_gData.value(GDTPhysics));
    }
    
    /**
     * @brief 获取网格数据并进行类型转换
     * @return T*
     */
    template<class T>
    T* getMeshData()
    {
        QMutexLocker locker(&_mutex);
        return dynamic_cast<T*>(_gData.value(GDTMesh));
    }
    
    /**
     * @brief 获取后处理数据并进行类型转换
     * @return T*
     */
    template<class T>
    T* getPostData()
    {
        QMutexLocker locker(&_mutex);
        return dynamic_cast<T*>(_gData.value(GDTPost));
    }
    
    
    /**
     * @brief 根据类型获取数据并进行类型转换
     * @param[i]  d        数据类型，参考FCGlobalDT枚举
     * @return T*
     */
    template<class T>
    T* getData(int d)
    {
        QMutexLocker locker(&_mutex);
        return dynamic_cast<T*>(_gData.value(d));
    }
    
    /**
     * @brief 获取全局数据ID
     * @return QList<int>
     */
    QList<int> getGlobalDataIDs();

    /** @brief 获取当前会话的全局数据（由 APP 在 createGlobalData 后设置） */
    static FCGlobalData* getGlobalData();
    /** @brief 设置当前会话的全局数据 */
    static void setCurrentGlobalData(FCGlobalData* g);

private:
    
    /**
     * @brief 插入数据管理
     * @param[i]  dt             数据类型，参考FITKGlobalDT枚举
     * @param[i]  d              数据对象
     */
    void insertData(int dt, FCAbstractDataObject* d);
    
private:
    /**
     * @brief 线程锁
     */
    static QMutex _mutex;
    
    /**
     * @brief 主界面指针
     */
    QWidget* _mainwindow{};
    
    /**
     * @brief 全局数据管理
     */
    QHash<int, FCAbstractDataObject*> _gData{};

    static FCGlobalData* s_currentGlobalData;
};

} // namespace FC

#endif // FCGLOBALDATA_H
